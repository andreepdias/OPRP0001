#ifndef __MATRIX_H
#define __MATRIX_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <math.h>
// #include "matrix_threads.h"
#include "quicksort.h"
// #include "quicksort_threads.h"



#define random() ((rand() ^ rand()) / (RAND_MAX + 1.0))

typedef struct {
      double **matrix;
      int rows;
      int cols;
} matrix_t;

/*
 * All functions must return a new matriz (when need)
 */


matrix_t *matrix_create(int rows, int cols);

void matrix_destroy(matrix_t *m);

matrix_t *matrix_clone(matrix_t *A);

void matrix_randfill(matrix_t *m);

void matrix_fill(matrix_t *m, double val);

void matrix_print(matrix_t *m);

matrix_t *matrix_multiply(matrix_t *A, matrix_t *B);

matrix_t *matrix_sum(matrix_t *A, matrix_t *B);

matrix_t *matrix_sort_bubble(matrix_t *A);

#endif 
