//gcc main.c matrix.c quicksort.c quicksort_threads.c matrix_threads.c -pthread -lm -o s

#include "matrix.h"
#include "matrix_threads.h"
#include "quicksort_threads.h"
#include "quicksort.h"

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

	if(nthreads == 0){
		printf("QuickSort Serial:\n");
	}else{
		printf("QuickSort Paralelo %d threads:\n", nthreads);
	}
	int i;
	for(i = 1; i <= 1; i++){

		matrix_t *A = matrix_create(nrows, ncols);
		matrix_randfill(A);

		matrix_t *M;

		start_time = wtime();
		if (nthreads == 0){
			M = matrix_sort_quick(A);
		}else{
			M = matrix_sort_quick_paralelo(A, nthreads);
		}
		end_time = wtime();

		matrix_print(A);
		matrix_destroy(A);
		
		double t = end_time - start_time;
		printf("%f\n", t);
	}
	return EXIT_SUCCESS;
}


double wtime(){
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec + t.tv_usec / 1000000.0;
}