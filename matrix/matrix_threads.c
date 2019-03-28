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

matrix_t *matrix_sum_paralelo(matrix_t *A, matrix_t *B, int nthreads){


	int i, k = 0, nrows = A->rows;
	int *bounds = (int*)malloc(sizeof(int) * nthreads);
	if(nthreads > nrows) nthreads = nrows;
	for(i = 0; i < nthreads; i++){
		bounds[i] = nrows / nthreads;
		k += bounds[i];
	}
	if(k != nrows){
		for(i = 0; i < nrows - k; i++){
			bounds[i]++;
		}
	}

	
	matrix_t *C = matrix_create(A->rows, A->cols);


	DadosThread *dt = NULL;
	pthread_t *threads = NULL;
	if (!(dt = (DadosThread *)malloc(sizeof(DadosThread) * nthreads)))
	{
		printf("Erro ao alocar memória\n");
		exit(EXIT_FAILURE);
	}
	if (!(threads = (pthread_t *)malloc(sizeof(pthread_t) * nthreads)))
	{
		printf("Erro ao alocar memória\n");
		exit(EXIT_FAILURE);
	}


	k = 0;
	for(i = 0; i < nthreads; i++){
		dt[i].id = i;
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