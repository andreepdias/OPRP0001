#ifndef __MATRIX_MERGESORT_H
#define __MATRIX_MERGESORT_H


#include "matrix.h"

void merge(double **matrix, int low, int mid, int high);
void mergeSort(double **matrix, int low, int high);
matrix_t *matrix_sort_merge(matrix_t *A);

void mergeSort_openmp(double **matrix, int low, int high, int current_level, int last_level);
matrix_t *matrix_sort_merge_openmp(matrix_t *A, int current_level, int last_level);


#endif