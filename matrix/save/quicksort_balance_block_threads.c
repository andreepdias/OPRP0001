#include "quicksort_threads.h"
#include "quicksort.h"
#include "matrix_threads.h"

void *quickSort_paralelo(void *_dt)
{
    DadosThreadQuickSort *dt = (DadosThreadQuickSort *)_dt;
    int low = dt->low, high = dt->high;
    int size_block = dt->size_block;
    double **matrix = dt->matrix;
    pthread_mutex_t *mutex_threads = dt->mutex_nthreads;

    int *nthreads = dt->nthreads;

    if (low < high)
    {
        int pi = partition(matrix, low, high);
        int n = high - low + 1;
        if (n <= size_block || (*nthreads) <= 1)
        {

            pthread_mutex_lock(mutex_threads);
            (*nthreads)--;
            pthread_mutex_unlock(mutex_threads);

            quickSort(matrix, low, pi - 1);
            quickSort(matrix, pi + 1, high);
        }
        else
        {
            DadosThreadQuickSort dt[2];
            pthread_t threads[2];
            int i;
            for (i = 0; i < 2; i++)
            {
                dt[i].matrix = matrix;
                dt[i].nthreads = nthreads;
                dt[i].mutex_nthreads = mutex_threads;
            }
            dt[0].low = low;
            dt[0].high = pi - 1;
            dt[1].low = pi + 1;
            dt[1].high = high;

            pthread_create(&threads[0], NULL, quickSort_paralelo, (void *)(dt + 0));
            pthread_create(&threads[1], NULL, quickSort_paralelo, (void *)(dt + 1));

            pthread_join(threads[0], NULL);
            pthread_join(threads[1], NULL);
        }
    }
}

matrix_t *matrix_sort_quick_paralelo(matrix_t *A, int nthreads)
{

    int depth = (int)log2(nthreads), n = A->rows * A->cols;

    pthread_mutex_t *mutex_nthreads = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(mutex_nthreads, NULL);

    DadosThreadQuickSort dt[1];
    dt[0].matrix = A->matrix;
    dt[0].low = 0;
    dt[0].high = n - 1;
    dt[0].size_block = n / nthreads;
    dt[0].nthreads = &nthreads;
    dt[0].mutex_nthreads = mutex_nthreads;

    quickSort_paralelo((void *)(dt + 0));


    pthread_mutex_destroy(mutex_nthreads);
    return A;
}
