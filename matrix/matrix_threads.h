#ifndef __MATRIX_THREADS_H
#define __MATRIX_THREADS_H

#include "matrix.h"


typedef struct {
	matrix_t *A;
	matrix_t *B;
	matrix_t *C;
	int lowerbound;
	int upperbound;
} DadosThread;

void *trabalho_thread_soma(void *);
void *trabalho_thread_multiplicao(void *);
matrix_t *matrix_sum_paralelo(matrix_t *A, matrix_t *B, int nthreads);
matrix_t *matrix_multiply_paralelo(matrix_t *A, matrix_t *B, int nthreads);

#endif