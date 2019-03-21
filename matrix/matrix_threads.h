#ifndef __MATRIX_THREADS_H
#define __MATRIX_THREADS_H

#include "matrix.h"


typedef struct {
	int id;
	matrix_t *A;
	matrix_t *B;
	matrix_t *C;
	int lowerbound;
	int upperbound;
} DadosThread;

void *trabalho_thread_soma(void *);
void *trabalho_thread_multiplicao(void *);


#endif