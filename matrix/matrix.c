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

matrix_t *matrix_multiply(matrix_t *A, matrix_t *B)
{
    matrix_t *m = matrix_create(A->rows, B->cols);
    matrix_fill(m, 0);

    int i, j, k;
    for(i = 0; i < A->rows; i++){
        for(j = 0; j < B->cols; j++){
            for(k = 0; k < A->cols; k++){
                m->matrix[i][j] += A->matrix[i][k] + B->matrix[k][j];
            }
        }
    }

    return m;
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

matrix_t *matrix_sum(matrix_t *A, matrix_t *B)
{
    if(A->rows != B->rows){
        printf("O numero de linhas das matrizes sao diferentes.");
    }else if(A->cols != B->cols){
        printf("O numero de colunas das matrizes sao diferentes.");
    }

    matrix_t *m = matrix_create(A->rows, A->cols);

    int i, j;
    for(i = 0; i < A->rows; i++){
        for(j = 0; j < A->cols; j++){
            m->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
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

void swap(double* a, double* b) 
{ 
    double t = *a; 
    *a = *b; 
    *b = t; 
} 
  
int partition (double **matrix, int low, int high) 
{ 
    double pivot = matrix[0][high];   
    int i = (low - 1);  
  
    for (int j = low; j <= high- 1; j++) 
    { 
        if (matrix[0][j] <= pivot) 
        { 
            i++;   
            swap(&matrix[0][i], &matrix[0][j]); 
        } 
    } 
    swap(&matrix[0][i + 1], &matrix[0][high]); 
    return (i + 1); 
} 

void quickSort(double **matrix, int low, int high) 
{ 
    if (low < high) 
    { 
        int pi = partition(matrix, low, high); 
  
        quickSort(matrix, low, pi - 1); 
        quickSort(matrix, pi + 1, high); 
    } 
} 

matrix_t *matrix_sort_quick(matrix_t *A)
{
    matrix_t *m = matrix_create(A->rows, A->cols);

    int i, j;
    for(i = 0; i < A->rows; i++){
        for(j = 0; j < A->cols; j++){
            m->matrix[i][j] = A->matrix[i][j];
        }
    }

    int n = A->rows * A->cols; 

    quickSort(m->matrix, 0, n - 1); 

    return m;
}
