#include "mergesort_threads.h"
#include "mergesort.h"
#include "matrix_threads.h"

void *mergeSort_paralelo(void *_dt)
{
    DadosThreadMergeSort *dt = (DadosThreadMergeSort *)_dt;
    int low = dt->low, high = dt->high;
    int current_level = dt->current_level, last_level = dt->last_level;
    double **matrix = dt->matrix;

    if (low < high)
    {
        int mid = low + (high - low) / 2;
        if (current_level < last_level)
        {

            DadosThreadMergeSort dt[2];
            pthread_t threads[2];
            int i;
            for (i = 0; i < 2; i++)
            {
                dt[i].matrix = matrix;
                dt[i].current_level = current_level + 1;
                dt[i].last_level = last_level;
            }
            dt[0].low = low;
            dt[0].high = mid;
            dt[1].low = mid + 1;
            dt[1].high = high;

            pthread_create(&threads[0], NULL, mergeSort_paralelo, (void *)(dt + 0));
            pthread_create(&threads[1], NULL, mergeSort_paralelo, (void *)(dt + 1));

            pthread_join(threads[0], NULL);
            pthread_join(threads[1], NULL);
        }
        else
        {
            mergeSort(matrix, low, mid);
            mergeSort(matrix, mid + 1, high);
        }
        merge(matrix, low, mid, high);
    }
}

matrix_t *matrix_sort_merge_paralelo(matrix_t *A, int nthreads)
{
    int depth = (int)log2(nthreads), n = A->rows * A->cols;

    DadosThreadMergeSort dt[1];
    dt[0].matrix = A->matrix;
    dt[0].low = 0;
    dt[0].high = n - 1;
    dt[0].current_level = 0;
    dt[0].last_level = depth;

    mergeSort_paralelo((void *)(dt + 0));

    return A;
}
