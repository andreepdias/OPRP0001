#ifndef __QUICKSORT_THREADS_H
#define __QUICKSORT_THREADS_H

#include "matrix.h"

typedef struct
{
	double **matrix;
	int low;
	int high;
	int size_block;
	int *nthreads;
	pthread_mutex_t *mutex_nthreads;
} DadosThreadQuickSort;

matrix_t *matrix_sort_quick_paralelo(matrix_t *A, int nthreads);

#endif