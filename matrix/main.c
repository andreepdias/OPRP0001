#include "matrix.h"
#include "matrix_threads.h"
// #include "quick_and_merge_threads.h"
#include "quicksort_threads.h"
#include "quicksort.h"
#include "mergesort.h"
#include "mergesort_threads.h"

double wtime();

int main(int argc, char *argv[])
{
	int nthreads, nrows, ncols, nruns;
	double start_time, end_time;

	if (argc < 5) {
		printf("%s <nthreads> <nruns> <rows> <cols>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	nthreads = atoi(argv[1]);
	nruns = atoi(argv[2]);
	nrows = atoi(argv[3]);
	ncols = atoi(argv[4]);

	int i, k = 0;
	double t, soma, media, desvio;

	for(k = 1; k <= 9; k++){
		if(k == 9) k = 16;
		nthreads = k;
		double tempos[nruns];
		if(nthreads == 0){
			printf("Multiply Serial:\n");
		}else{
			printf("Multiply Paralelo %d threads:\n", nthreads);
		}
		for (i = 0; i < nruns; i++)
		{

			matrix_t *A = matrix_create(nrows, ncols);
			matrix_randfill(A);
			matrix_t *B = matrix_create(nrows, ncols);
			matrix_randfill(B);

			matrix_t *M = matrix_create(nrows, ncols);

			start_time = wtime();
			if (nthreads == 0){
				M = matrix_multiply(A, B, M);
			}else{
				M = matrix_multiply_paralelo(A, B, M, nthreads);
			}
			end_time = wtime();

			// matrix_print(A);
			matrix_destroy(A);
			matrix_destroy(B);
			matrix_destroy(M);
			
			double t = end_time - start_time;
			tempos[i] = t;
			soma += t;
			printf("%lf\n",t);
		}
		media = soma / nruns;
		for (i = 0; i < nruns; i++){
			desvio += pow((tempos[i] - media), 2);
		}
		desvio = sqrt(desvio / nruns);
		printf("Media:\t%lf\nDesvio:\t%lf\n", media, desvio);
	}


	return EXIT_SUCCESS;
}


double wtime(){
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec + t.tv_usec / 1000000.0;
}