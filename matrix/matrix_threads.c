#include "matrix.h"
#include "matrix_threads.h"


void *trabalho_thread_soma(void *_dt)
{
	DadosThread *dt = (DadosThread*) _dt;
	
	int nrows, ncols, lowerbound, upperbound;
	nrows = dt->A->rows;
	ncols = dt->A->cols;
	lowerbound = dt->lowerbound;
	upperbound = dt->upperbound;

	int i, j;
	for(i = lowerbound; i < upperbound; i++){
		for(j = 0; j < ncols; j++){
			dt->C->matrix[i][j] = dt->A->matrix[i][j] + dt->B->matrix[i][j];
		}
	}
	return NULL;
}

void *trabalho_thread_multiplicao(void *_dt)
{
	DadosThread *dt = (DadosThread*) _dt;
	
	int arows, acols, brows, bcols, lowerbound, upperbound;
	arows = dt->A->rows;
	acols = dt->A->cols;
	brows = dt->B->rows;
	bcols = dt->B->cols;
	lowerbound = dt->lowerbound;
	upperbound = dt->upperbound;

	int i, j, k;
	for(i = lowerbound; i < upperbound; i++){
		for(j = 0; j < bcols; j++){
			for(k = 0; k < acols; k++){
				dt->C->matrix[i][j] += dt->A->matrix[i][k] * dt->B->matrix[k][j];
			}
		}
	}
	return NULL;
}

