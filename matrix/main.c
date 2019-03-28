//gcc main.c matrix.c quicksort.c quicksort_threads.c matrix_threads.c -pthread -lm -o s

#include "matrix.h"
#include "matrix_threads.h"
#include "quicksort_threads.h"
#include "quicksort.h"

double wtime();

int main(int argc, char *argv[])
{
	srand(time(NULL));

	int nthreads, nrows, ncols;
	double start_time, end_time;

	if (argc < 4) {
		printf("%s <nthreads> <rows> <cols>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	nthreads = atoi(argv[1]);
	nrows = atoi(argv[2]);
	ncols = atoi(argv[3]);

	double s = 0.0, t, m;

	printf("Quick Paralelo %d Threads:\n", nthreads);
	// printf("Quick Serial:\n");
	int i;
	for(i = 1; i <= 1; i++){

		matrix_t *A = matrix_create(nrows, ncols);
		matrix_randfill(A);
		matrix_t *B = matrix_create(nrows, ncols);
		matrix_randfill(B);

		start_time = wtime();
		// matrix_t *M = matrix_sum(A, B);
		// matrix_t *M = matrix_sum_paralelo(A, B, nthreads);
		// matrix_t *M = matrix_multiply(A, B);
		matrix_t *M = matrix_multiplicacao_paralelo(A, B, nthreads);
		// matrix_t *M = matrix_sort_quick(A);
		// matrix_t *M = matrix_sort_quick_paralelo(A, nthreads);
		end_time = wtime();

		// matrix_print(A);
		// matrix_print(B);
		// matrix_print(M);

		matrix_destroy(A);
		matrix_destroy(B);
		matrix_destroy(M);

		
		t = end_time - start_time;
		s += t;
		printf("%f\n", t);
	}

	m = s / 10;
	printf("\nMedia:\n%lf\n", m);

	return EXIT_SUCCESS;
	
	
}


double wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec + t.tv_usec / 1000000.0;
}