#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "matrix.h"

double wtime(){
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec + t.tv_usec / 1000000.0;
}

int main(int argc, char **argv){
   double start_time, end_time;
    int nrows, ncols;

   if ((argc != 4)) {
      printf("Uso: %s <rows> <cols> <threads>\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   nrows = atoi(argv[1]);
   ncols = atoi(argv[2]);
   int nthreads = atoi(argv[3]);

   for(int i = 0; i < 10; i++)
   {
      start_time = wtime();

      matrix_t *a = matrix_create_block(nrows, ncols);
      matrix_t *b = matrix_create_block(nrows, ncols);

      // matrix_t *res = matrix_sum_parallel(tmp_block, tmp_block, matrix_create_block, nthreads);
      // matrix_t *res = matrix_multiply_parallel(tmp_block, tmp_block, matrix_create_block, nthreads);
      // matrix_t *res = matrix_sum(a, b, matrix_create_block);
      // matrix_t *res = matrix_sum_parallel(a, b, matrix_create_block, nthreads);
      // matrix_t *res = matrix_multiply(tmp_block, tmp_block, matrix_create_block);
      matrix_print(a);

      matrix_destroy_block(a);
      matrix_destroy_block(b);
      // matrix_destroy_block(res);

      // matrix_destroy_pointers(tmp_pointers);

      end_time = wtime();
      printf("%.4lf\n", end_time - start_time);
   }

   printf("%d %d %f\n", nrows, ncols, end_time - start_time);
   fflush(stdout);

   return EXIT_SUCCESS;
}
