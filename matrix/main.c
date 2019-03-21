//gcc main.c matrix.c quicksort.c quicksort_threads.c matrix_threads.c -pthread -lm

#include "matrix.h"
#include "quicksort_threads.h"

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

	start_time = wtime();


	matrix_t *A = matrix_create(nrows, ncols);
	matrix_randfill(A);
	matrix_print(A);

	matrix_t *M = matrix_sort_quick_paralelo(A, nthreads);
	matrix_print(M);

	// end_time = wtime();
	// printf("%f s\n", end_time - start_time);
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


/*
	free(dt);
	free(threads);
	matrix_destroy(a);
    matrix_destroy(c);
	

	matrix_t *a = matrix_create(nrows, ncols);
    matrix_fill(a, 2);
	matrix_t *b = matrix_create(nrows, nc
	// end_time = wtime();
	// printf("%f s\n", end_time - start_time);ols);
    matrix_fill(b, 2);
	matrix_t *c = matrix_create(nrows, ncols);
	matrix_fill(c, 0.0);
		// end_time = wtime();
	// printf("%f s\n", end_time - start_time);

	// end_time = wtime();
	// printf("%f s\n", end_time - start_time);

	// end_time = wtime();
	// printf("%f s\n", end_time - start_time);

	// end_time = wtime();
	// printf("%f s\n", end_time - start_time);

	// end_time = wtime();
	// printf("%f s\n", end_time - start_time);

	// end_time = wtime();
	// printf("%f s\n", end_time - start_time);

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
	f
		// end_time = wtime();s[i]);
	
	}	// printf("%f s\n", end_time - start_time);
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

	*/

	end_time = wtime();
	printf("%f s\n", end_time - start_time);

	return EXIT_SUCCESS;
}


double wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec + t.tv_usec / 1000000.0;
}