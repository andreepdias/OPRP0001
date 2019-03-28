#include "matrix.h"
#include "matrix_threads.h"


void *trabalho_thread_soma(void *_dt){
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
	pthread_exit(NULL);
}

void *trabalho_thread_multiplicao(void *_dt){
	DadosThread *dt = (DadosThread*) _dt;
	
	int arows, acols, brows, bcols, lowerbound, upperbound;
	arows = dt->A->rows, acols = dt->A->cols;
	brows = dt->B->rows, bcols = dt->B->cols;
	lowerbound = dt->lowerbound, upperbound = dt->upperbound;

	int i, j, k;
	for(i = lowerbound; i < upperbound; i++){
		for(j = 0; j < bcols; j++){
			for(k = 0; k < acols; k++){
				dt->C->matrix[i][j] += dt->A->matrix[i][k] * dt->B->matrix[k][j];
			}
		}
	}
	pthread_exit(NULL);
}

int *divide_em_partes(int nrows, int nthreads){
	int i, k = 0;
	int *bounds = (int *)malloc(sizeof(int) * nthreads);
	if (nthreads > nrows)
		nthreads = nrows;
	for (i = 0; i < nthreads; i++){
		bounds[i] = nrows / nthreads;
		k += bounds[i];
	}
	if (k != nrows){
		for (i = 0; i < nrows - k; i++)
		{
			bounds[i]++;
		}
	}
	return bounds;
}

matrix_t *matrix_sum_paralelo(matrix_t *A, matrix_t *B, int nthreads){

	int *bounds = divide_em_partes(A->rows, nthreads);
	
	matrix_t *C = matrix_create(A->rows, A->cols);

	DadosThread dt[nthreads];
	pthread_t threads[nthreads];

	int k = 0, i = 0;
	for(i = 0; i < nthreads; i++){
		dt[i].A = A;
		dt[i].B = B;
		dt[i].C = C;
		dt[i].lowerbound = k;
		dt[i].upperbound = bounds[i] + k;
		k = dt[i].upperbound;
		pthread_create(&threads[i], NULL, trabalho_thread_soma, (void *)(dt + i));
	}
	for (i = 0; i < nthreads; i++) {
		pthread_join(threads[i], NULL);		
	}
	return C;
}

matrix_t *matrix_multiply_paralelo(matrix_t *A, matrix_t *B, int nthreads)
{

	int *bounds = divide_em_partes(A->rows, nthreads);

	matrix_t *C = matrix_create(A->rows, A->cols);

	DadosThread dt[nthreads];
	pthread_t threads[nthreads];

	int i = 0, k = 0;
	for(i = 0; i < nthreads; i++){
		dt[i].A = A;
		dt[i].B = B;
		dt[i].C = C;
		dt[i].lowerbound = k;
		dt[i].upperbound = bounds[i] + k;
		k = dt[i].upperbound;
		pthread_create(&threads[i], NULL, trabalho_thread_multiplicao, (void *)(dt + i));
	}
	for (i = 0; i < nthreads; i++) {
		pthread_join(threads[i], NULL);		
	}
	return C;
}