#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "matrix.h"

matrix_t *matrix_create(int rows, int cols)
{
    matrix_t *m = (matrix_t*)malloc(sizeof(matrix_t));

    double *temp = (double*)malloc(sizeof(double) * rows * cols);

    m->rows = rows;
    m->cols = cols;
    m->data = malloc(sizeof(double *) * rows);

    int i = 0;
    for(i = 0; i < rows; i++){
        m->data[i] = &temp[i * cols];
    }

    return m;
}

void matrix_destroy(matrix_t *m)
{
    free(m->data[0]);
    free(m->data);
    free(m);

    m = NULL;
}

void matrix_randfill(matrix_t *m)
{
   int i, j;
   for (i = 0; i < m->rows; i++) {
      for (j = 0; j < m->cols; j++) {
         m->data[i][j] = random();
      }
   }
}

void matrix_fill(matrix_t *m, double val)
{
   int i, j;
   for (i = 0; i < m->rows; i++) {
      for (j = 0; j < m->cols; j++) {
         m->data[i][j] = val;
      }
   }
}

matrix_t *matrix_multiply(matrix_t *A, matrix_t *B)
{
    if(A->cols != B->rows){
        printf("O numero de colunas de A e diferente do numero de linhas de B.");
    }

    matrix_t *m = matrix_create(A->rows, B->cols);
    matrix_fill(m, 0);

    int i, j, k;
    for(i = 0; i < A->rows; i++){
        for(j = 0; j < B->cols; j++){
            for(k = 0; k < A->cols; k++){
                m->data[i][j] += A->data[i][k] + B->data[k][j];
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
         printf("%.17f ", m->data[i][j]);
      }
      printf("\n");
   }
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
            m->data[i][j] = A->data[i][j] + B->data[i][j];
        }
    }

    return m;
}

matrix_t *matrix_sort(matrix_t *A)
{
    matrix_t *m = matrix_create(A->rows, A->cols);

    int i, j;
    for(i = 0; i < A->rows; i++){
        for(j = 0; j < A->cols; j++){
            m->data[i][j] = A->data[i][j];
        }
    }

    double aux;
    for(i = 0; i < A->rows * A->cols; i++){
        for(j = i; j < A->rows * A->cols; j++){
            if(m->data[0][j] < m->data[0][i]){
                aux = m->data[0][i];
                m->data[0][i] = m->data[0][j];
                m->data[0][j] = aux;
            }
        }
    }

    return m;
}
