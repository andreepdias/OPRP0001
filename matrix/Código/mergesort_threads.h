#ifndef __MERGESORT_THREADS_H
#define __MERGESORT_THREADS_H

#include "matrix.h"

typedef struct
{
	double **matrix;
	int low;
	int high;
	int current_level;
	int last_level;
} DadosThreadMergeSort;

matrix_t *matrix_sort_merge_paralelo(matrix_t *A, int nthreads);

#endif