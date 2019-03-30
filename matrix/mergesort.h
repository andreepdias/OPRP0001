#ifndef __MATRIX_MERGESORT_H
#define __MATRIX_MERGESORT_H


#include "matrix.h"

void merge(double **matrix, int low, int mid, int high);
void mergeSort(double **matrix, int low, int high);
matrix_t *matrix_sort_merge(matrix_t *A);

#endif