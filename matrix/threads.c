#include "matrix.h"
#include <math.h>

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
double wtime();


int main(int argc, char *argv[])
{
	int nthreads, nrows, ncols;
	double start_time, end_time;

	if (argc < 4) {
		printf("%s <nthreads> <rows> <cols>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	nthreads = atoi(argv[1]);
	nrows = atoi(argv[2]);
	ncols = atoi(argv[3]);

	/*
	DadosThread *dt = NULL;
	pthread_t *threads = NULL;

	if (!(dt = (DadosThread *) malloc(sizeof(DadosThread) * nthreads))) {
		printf("Erro ao alocar memória\n");
		exit(EXIT_FAILURE);
	}
	if (!(threads = (pthread_t *) malloc(sizeof(pthread_t) * nthreads))) {
		 printf("Erro ao alocar memória\n");
                exit(EXIT_FAILURE);
	}
	*/

	start_time = wtime();

/*
	free(dt);
	free(threads);
	matrix_destroy(a);
    matrix_destroy(c);
	

	matrix_t *a = matrix_create(nrows, ncols);
    matrix_fill(a, 2);
	matrix_t *b = matrix_create(nrows, ncols);
    matrix_fill(b, 2);
	matrix_t *c = matrix_create(nrows, ncols);
	matrix_fill(c, 0.0);
	

	matrix_print(a);
	matrix_print(b);
	matrix_print(c);

	int i, k = 0;
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
	for(i = 0; i < nthreads; i++){
		printf("Bounds[%d]: %d\n", i, bounds[i]);
	
	}
	k = 0;
	for(i = 0; i < nthreads; i++){
		dt[i].id = i;
		dt[i].A = a;
		dt[i].B = b;
		dt[i].C = c;
		dt[i].lowerbound = k;
		dt[i].upperbound = bounds[i] + k;
		k = dt[i].upperbound;
	 	pthread_create(&threads[i], NULL, trabalho_thread_multiplicao, (void *) (dt + i));

	}

	for (i = 0; i < nthreads; i++) {
		pthread_join(threads[i], NULL);		
	}
	
	printf("a:\n");
	matrix_print(a);
	printf("b:\n");
	matrix_print(b);
	printf("c:\n");
	matrix_print(c);
	

	free(bounds);
	free(dt);
	free(threads);
	matrix_destroy(a);
    matrix_destroy(b);
	matrix_destroy(c);

	*/

	end_time = wtime();
	printf("%f s\n", end_time - start_time);

	return EXIT_SUCCESS;
}

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


double wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec + t.tv_usec / 1000000.0;
}