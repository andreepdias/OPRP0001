#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include "matrix.h"


typedef struct {
    int start, end;
    matrix_t *A;
    matrix_t *B;
    matrix_t *res;
} DadosThread;

matrix_t *matrix_create_block(int rows, int cols){
    matrix_t *tmp = (matrix_t *)malloc(sizeof(matrix_t));
    double *mat = (double*) malloc(sizeof(double) * rows * cols);
    tmp -> rows = rows;
    tmp -> cols = cols;
    tmp -> data = (double **)malloc(sizeof(double *) * rows);
    for(int i = 0; i < rows; i++){
        tmp -> data[i] = mat + i * cols;
    }
    return tmp;
}

matrix_t *matrix_create_pointers(int rows, int cols){
    matrix_t *tmp = (matrix_t *)malloc(sizeof(matrix_t));
    tmp -> rows = rows;
    tmp -> cols = cols;
    tmp -> data = (double **)malloc(sizeof(double *) * rows);
    for(int i = 0; i < rows; i++){
        tmp -> data[i] = (double *)malloc(sizeof(double) * cols);
    }
    return tmp;
}

void *create_cols_thread(void *args) {
	DadosThread *dados = (DadosThread*) args;
	int inicio = dados->start, fim = dados->end;
	matrix_t *m = dados->A;
	for (int i = inicio; i < fim; ++i)
		m->data[i] = (double *) malloc(sizeof(double) * m->cols);
    pthread_exit(NULL);
}

matrix_t *matrix_create_pointers_parallel(int rows, int cols, int num_threads){
    matrix_t *tmp = (matrix_t *)malloc(sizeof(matrix_t));
    tmp -> rows = rows;
    tmp -> cols = cols;
    tmp -> data = (double **)malloc(sizeof(double *) * rows);
    DadosThread dados[num_threads];
    pthread_t threads[num_threads];
    for(int i = 0; i < rows; i++){
	   dados[i].start = i * (rows / num_threads);
	   dados[i].end = i < rows - 1 ? (i + 1) * (rows / num_threads) : rows;
	   dados[i].A = tmp;
	   pthread_create(&threads[i], NULL, create_cols_thread, (void *)&dados[i]);
	}
	for(int i = 0; i < num_threads; i++){
	   pthread_join(threads[i], NULL);
	}

    return tmp;
}

void matrix_destroy_pointers(matrix_t *m){
    if(m == NULL) return;
    for(int i = 0; i < m -> rows; i++){
        free(m -> data[i]);
    }
    free(m -> data);
    return;
}

void matrix_destroy_block(matrix_t *m) {
    if(m == NULL) return;
    free(*(m->data));
    free(m->data);
    return;
}

void matrix_randfill(matrix_t *m){
    if(m == NULL) return;
   int i, j;
   for (i = 0; i < m->rows; i++) {
      for (j = 0; j < m->cols; j++) {
         m->data[i][j] = random();
      }
   }
}

void matrix_fill(matrix_t *m, double val){
    if(m == NULL) return;
    int i, j;
    for (i = 0; i < m->rows; i++) {
        for (j = 0; j < m->cols; j++) {
            m->data[i][j] = val;
       }
    }
}

matrix_t *matrix_multiply(matrix_t *A, matrix_t *B, matrix_t *p (int, int)) {
    if(A == NULL || B == NULL || A -> cols != B -> rows) return NULL;

    matrix_t *tmp = p(A -> rows, B -> cols);
    for(int i = 0; i < A -> rows; i++){
        for(int j = 0; j < A -> cols; j++){
            double s = 0;
            for(int k = 0; k < B -> cols; k++){
                s += (A -> data[i][k] * B -> data[k][j]);
            }
            tmp -> data[i][j] = s;
        }
    }

    return tmp;
}

void *matrix_multiply_thread(void *args){
    DadosThread *dadosThread = (DadosThread *)args;

    int inicio = dadosThread -> start, fim = dadosThread->end;
    matrix_t *A = dadosThread -> A;
    matrix_t *B = dadosThread -> B;
    matrix_t *res = dadosThread -> res;
    for (int i = inicio; i < fim; ++i) {
        for(int j = 0; j < A -> cols; j++){
            long long sum = 0;
            for(int k = 0; k < B -> rows; k++){
                sum += A -> data[i][k] * B -> data[k][j];
            }
            res -> data[i][j] = sum;
        }
    }
    pthread_exit(NULL);
}

matrix_t *matrix_multiply_parallel(matrix_t *A, matrix_t *B, matrix_t *p (int, int), int num_threads) {
    if(A == NULL || B == NULL || A -> cols != B -> rows) return NULL;

    matrix_t *tmp = p(A->rows, A->cols);
    pthread_t threads[num_threads];
    DadosThread dado[num_threads];
    for(int i = 0; i < num_threads; i++){
       dado[i].start = i * (A->rows / num_threads);
       dado[i].end = i < A->rows - 1 ? (i + 1) * (A->rows / num_threads) : A->rows;
       dado[i].A = A;
       dado[i].B = B;
       dado[i].res = tmp;
       pthread_create(&threads[i], NULL, matrix_multiply_thread, (void *)&dado[i]);
    }
    for(int i = 0; i < num_threads; i++){
       pthread_join(threads[i], NULL);
    }
    return tmp;
}



void matrix_print(matrix_t *m){
    if(m == NULL) return;

   int i, j;
   for (i = 0; i < m->rows; i++) {
      for (j = 0; j < m->cols; j++) {
         printf("%.17f ", m->data[i][j]);
      }
      printf("\n");
   }
   fflush(stdout);
}

int matrix_equal(matrix_t*A, matrix_t *B){
   int i, j;

   if (A == NULL || B == NULL || A->rows != B->rows || A->cols != B->cols)
      return 0;

   for (i = 0; i < A->rows; i++) {
      for (j = 0; j < A->cols; j++) {
         if (A->data[i][j] != B->data[i][j])
            return 0;
      }
   }
   return 1;
}

matrix_t *matrix_sum(matrix_t *A, matrix_t *B, matrix_t *p (int, int)) {
    if(A == NULL || B == NULL || A -> rows != B -> rows || A -> cols != B -> cols) return NULL;

    matrix_t *tmp = p(A->rows, A->cols);
    for (int i = 0; i < A->rows; ++i) {
        for (int j = 0; j < A->cols; ++j) {
            tmp->data[i][j] = A->data[i][j] + B->data[i][j];
        }
    }
    return tmp;
}

void *matrix_sum_thread(void *args){
    DadosThread *dadosThread = (DadosThread *)args;

    int inicio = dadosThread -> start, fim = dadosThread->end;
    matrix_t *A = dadosThread -> A;
    matrix_t *B = dadosThread -> B;
    matrix_t *res = dadosThread -> res;
    for (int i = inicio; i < fim; ++i) {
        for(int j = 0; j < A -> cols; j++){
            res -> data[i][j] = A -> data[i][j] + B -> data[i][j];
        }
    }
    pthread_exit(NULL);
}

matrix_t *matrix_sum_parallel(matrix_t *A, matrix_t *B, matrix_t *p (int, int), int num_threads) {
    if(A == NULL || B == NULL || A -> rows != B -> rows || A -> cols != B -> cols) return NULL;

    matrix_t *tmp = p(A->rows, A->cols);
    pthread_t threads[num_threads];
    DadosThread dado[num_threads];
    for(int i = 0; i < num_threads; i++){
       dado[i].start = i * (A->rows / num_threads);
       dado[i].end = i < A->rows - 1 ? (i + 1) * (A->rows / num_threads) : A->rows;
       dado[i].A = A;
       dado[i].B = B;
       dado[i].res = tmp;
       pthread_create(&threads[i], NULL, matrix_sum_thread, (void *)&dado[i]);
    }
    for(int i = 0; i < num_threads; i++){
       pthread_join(threads[i], NULL);
    }
    return tmp;
}

matrix_t *matrix_inversion(matrix_t *A, matrix_t *create (int, int)) {
    if(A == NULL || A -> rows != A -> cols) return NULL;
    matrix_t *tmp = create(A -> rows, A -> cols);
    double det = matrix_determinant(A, create);
    if(det == 0) return NULL;
    for(int i = 0; i < A -> rows; i++){
        for(int j = 0; j < A -> cols; j++){
            matrix_t *aux = matrix_create_pointers(A -> rows - 1, A -> cols - 1);
            int linha = 0;
            for(int ii = 0; ii < A -> rows; ii++){
                int coluna = 0;
                if(ii == i) continue;
                for(int jj = 0; jj < A -> cols; jj++){
                    if(jj == j) continue;
                    aux -> data[linha][coluna++] = A -> data[ii][jj];
                }
                linha++;
            }
            double det2 = matrix_determinant(aux, create);
            tmp -> data[i][j] = ((i + j) % 2 == 0 ? 1 : -1) * det2 / det;
            matrix_destroy_pointers(aux);
        }
    }
    return tmp;
}

matrix_t *matrix_transpose(matrix_t *A, matrix_t *p (int, int)) {
    if(A == NULL) return NULL;
    matrix_t *tmp = p(A -> cols, A -> rows);
    for(int i = 0; i < A -> rows; i++){
        for(int j = 0; j < A -> cols; j++){
            tmp -> data[j][i] = A -> data[i][j];
        }
    }
    return tmp;
}

double matrix_determinant(matrix_t *A, matrix_t *create (int, int)) {
    if(A == NULL || A -> rows != A -> cols) return -1e9;
    if(A -> rows == 1) return A -> data[0][0];
    if(A -> rows == 2){
        return (A -> data[0][0] * A -> data[1][1] - (A -> data[1][0] * A -> data[0][1]));
    }
    double res = 0;
    for(int i = 0; i < A -> cols; i++){
        matrix_t *tmp = matrix_create_pointers(A -> rows - 1, A -> cols - 1);
        for(int j = 1; j < A -> rows; j++){
            int cnt = 0;
            for(int k = 0; k < A -> cols; k++){
                if(k == i) continue;
                tmp -> data[j - 1][cnt++] = A -> data[j][k];
            }
        }
        res += (A -> data[0][i] * matrix_determinant(tmp, create)) * (i % 2 == 0 ? 1 : -1);
        matrix_destroy_pointers(tmp);
    }
    return res;
}

matrix_t *createU(matrix_t *A){
    matrix_t *res = matrix_create_pointers(A -> rows, A -> cols);
    for(int i = 0; i < A -> rows; i++){
        for(int j = 0; j < A -> cols; j++){
            res -> data[i][j] = A -> data[i][j];
        }
    }
    for(int i = 1; i < A -> rows; i++){
        for(int k = i; k < A -> rows; k++){
            double fat = res -> data[k][i - 1] / (double)res -> data[i - 1][i - 1];
            for(int j = i - 1; j < A -> cols; j++){
                res -> data[k][j] = res -> data[k][j] - res -> data[i - 1][j] * fat;
            }
        }
    }
    // matrix_print(res);
    return res;
}

double matrix_determinant_LU(matrix_t *A, int num_threads){
    matrix_t *U = createU(A);
    double res = 1;
    for(int i = 0; i < A -> rows; i++){
        res *= U -> data[i][i];
    }
    return (res);
}
