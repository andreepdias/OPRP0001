//escalonamento / tamanho pedaço / flag compilação

#include "matrix.h"

matrix_t *matrix_create(int rows, int cols)
{
    matrix_t *m = (matrix_t*)malloc(sizeof(matrix_t));
    double *temp = (double*)malloc(sizeof(double) * rows * cols);

    m->rows = rows;
    m->cols = cols;
    m->matrix = malloc(sizeof(double *) * rows);

    int i = 0;
    for(i = 0; i < rows; i++){
        m->matrix[i] = &temp[i * cols];
    }
    
    return m;
}

void matrix_destroy(matrix_t *m)
{
    free(m->matrix[0]);
    free(m->matrix);
    free(m);
    m = NULL;
}

void matrix_randfill(matrix_t *m)
{
    srand(time(NULL));
   int i, j;
   for (i = 0; i < m->rows; i++) {
      for (j = 0; j < m->cols; j++) {
         m->matrix[i][j] = random();
      }
   }
}

void matrix_fill(matrix_t *m, double val)
{
   int i, j;
   for (i = 0; i < m->rows; i++) {
      for (j = 0; j < m->cols; j++) {
         m->matrix[i][j] = val;
      }
   }
}

matrix_t *matrix_sum(matrix_t *A, matrix_t *B, matrix_t *M)
{
    int i, j;
    for(i = 0; i < A->rows; i++){
        for(j = 0; j < A->cols; j++){
            M->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
        }
    }
    return M;
}

matrix_t *matrix_sum_openmp(matrix_t *A, matrix_t *B, matrix_t *M)
{
    int i, j;
    #pragma omp for schedule(dynamic)
    for(i = 0; i < A->rows; i++){
        for(j = 0; j < A->cols; j++){
            M->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
        }
    }
    return M;
}

matrix_t *matrix_multiply(matrix_t *A, matrix_t *B, matrix_t *M)
{
    int i, j, k;
    for(i = 0; i < A->rows; i++){
        for(j = 0; j < B->cols; j++){
            M->matrix[i][j] = 0;
            for(k = 0; k < A->cols; k++){
                M->matrix[i][j] += A->matrix[i][k] + B->matrix[k][j];
            }
        }
    }
    return M;
}

matrix_t *matrix_multiply_openmp(matrix_t *A, matrix_t *B, matrix_t *M)
{
    int i, j, k;
    #pragma omp for schedule(dynamic)
    for(i = 0; i < A->rows; i++){
        for(j = 0; j < B->cols; j++){
            M->matrix[i][j] = 0;
            for(k = 0; k < A->cols; k++){
                M->matrix[i][j] += A->matrix[i][k] + B->matrix[k][j];
            }
        }
    }
    return M;
}

void matrix_print(matrix_t *m)
{
   int i, j;
   for (i = 0; i < m->rows; i++) {
      for (j = 0; j < m->cols; j++) {
         printf("%.17f ", m->matrix[i][j]);
      }
      printf("\n");
   }
   printf("\n");
   fflush(stdout);
}

matrix_t *matrix_clone(matrix_t *A){
    matrix_t *m = matrix_create(A->rows, A->cols);

    int i, j;
    for(i = 0; i < A->rows; i++){
        for(j = 0; j < A->cols; j++){
            m->matrix[i][j] = A->matrix[i][j];
        }
    }
    return m;
}

matrix_t *matrix_sort_bubble(matrix_t *A)
{
    matrix_t *m = matrix_create(A->rows, A->cols);

    int i, j;
    for(i = 0; i < A->rows; i++){
        for(j = 0; j < A->cols; j++){
            m->matrix[i][j] = A->matrix[i][j];
        }
    }

    double aux;
    for(i = 0; i < A->rows * A->cols; i++){
        for(j = i; j < A->rows * A->cols; j++){
            if(m->matrix[0][j] < m->matrix[0][i]){
                aux = m->matrix[0][i];
                m->matrix[0][i] = m->matrix[0][j];
                m->matrix[0][j] = aux;
            }
        }
    }

    return m;
}
