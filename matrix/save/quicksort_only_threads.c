#include "quicksort_threads.h"
#include "quicksort.h"
#include "matrix_threads.h"


void *quickSort_paralelo(void *_dt)
{
    DadosThreadQuickSort *dt = (DadosThreadQuickSort*) _dt;
    int low = dt->low, high = dt->high;
    int current_level = dt->current_level, last_level = dt->last_level;
    double **matrix = dt->matrix;

    if (low < high)
    {
        int pi = partition(matrix, low, high);
        int n = high - low;
        if(current_level < last_level){

            DadosThreadQuickSort dt[2];
            pthread_t threads[2];
            int i;
            for (i = 0; i < 2; i++)
            {
                dt[i].matrix = matrix;
                dt[i].current_level = current_level + 1;
                dt[i].last_level = last_level;
            }
            dt[0].low = low;
            dt[0].high = pi - 1;
            dt[1].low = pi + 1;
            dt[1].high = high;

            pthread_create(&threads[0], NULL, quickSort_paralelo, (void *)(dt + 0));
            pthread_create(&threads[1], NULL, quickSort_paralelo, (void *)(dt + 1));

            pthread_join(threads[0], NULL);
            pthread_join(threads[1], NULL);
        }else{
            quickSort(matrix, low, pi - 1);
            quickSort(matrix, pi + 1, high);
        }

    }
}

matrix_t *matrix_sort_quick_paralelo(matrix_t *A, int nthreads)
{
    int depth = (int)log2(nthreads), n = A->rows * A->cols;

        DadosThreadQuickSort dt[1];
        dt[0].matrix = A->matrix;
        dt[0].low = 0;
        dt[0].high = n - 1;
        dt[0].current_level = 0;
        dt[0].last_level = depth;


        quickSort_paralelo((void *)(dt + 0));

        return A;
}

