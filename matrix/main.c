#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "matrix.h"

double wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec + t.tv_usec / 1000000.0;
}

int main(int argc, char **argv)
{
   double start_time, end_time;
    int nrows, ncols;

   if ((argc != 3)) {
      // printf("Uso: %s <rows> <cols>\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   nrows = atoi(argv[1]);
   ncols = atoi(argv[2]);

   start_time = wtime();

    // printf("Matriz A:\n");
    matrix_t *a = matrix_create(nrows, ncols);
    matrix_randfill(a);
    // matrix_print(a);

    // printf("\n\nMatriz B:\n");
    matrix_t *b = matrix_create(nrows, ncols);
    matrix_randfill(b);
    // matrix_print(b);

    // printf("\n\nMatriz SOMA:\n");
    matrix_t *soma = matrix_sum(a, b);
    // matrix_print(soma);

    // printf("\n\nMatriz A:\n");
    // matrix_fill(a, 1);
    // matrix_print(a);

    // printf("\n\nMatriz B:\n");
    // matrix_fill(b, 2);
    // matrix_print(b);

    // printf("\n\nMatriz MULT:\n");
    matrix_t *mult = matrix_multiply(a, b);
    // matrix_print(mult);

    // printf("\n\nMatriz S:\n");
    // matrix_randfill(a);
    // matrix_print(a);

    // printf("\n\nMatriz SORT:\n");
    matrix_t *sort = matrix_sort(a);
    // matrix_print(sort);



    matrix_destroy(a);
    matrix_destroy(b);
    matrix_destroy(soma);
    matrix_destroy(mult);
    matrix_destroy(sort);

   end_time = wtime();

   printf("\n%d %d %f\n", nrows, ncols, end_time - start_time);
   fflush(stdout);

   return EXIT_SUCCESS;
}
