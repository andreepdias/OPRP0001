#ifndef __MATRIX_QUICKSORT_H
#define __MATRIX_QUICKSORT_H


#include "matrix.h"

void quickSort(double **matrix, int low, int high);
matrix_t *matrix_sort_quick(matrix_t *A);
void swap(double *a, double *b);
int partition(double **matrix, int low, int high);

void quickSort_openmp(double **matrix, int low, int high, int current_level, int last_level);
matrix_t *matrix_sort_quick_openmp(matrix_t *A, int current_level, int last_level);

#endif