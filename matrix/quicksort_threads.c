#include "quicksort_threads.h"
#include "quicksort.h"
#include "matrix_threads.h"

void *partition_swap(void *_dt){
    DadosThreadPartition *dt = (DadosThreadPartition*) _dt;

    int low = dt->low, high = dt->high, pivot = dt->pivot;
    int *i = dt->i;
    double **matrix = dt->matrix;
    pthread_mutex_t *mutex_i = dt->mutex_i;

    for (int j = low; j < high; j++){
        if (matrix[0][j] < pivot){
            pthread_mutex_lock(mutex_i);
            (*i)++;
            pthread_mutex_unlock(mutex_i);
            swap(&matrix[0][(*i)], &matrix[0][j]);
        }
    }
}


int partition_paralelo(double **matrix, int low, int high, int current_level, int last_level)
{
    int random = low + rand() % (high - low);
    swap(&matrix[0][random], &matrix[0][high]);

    double pivot = matrix[0][high];
    int *i;
    (*i) = low - 1;

    int nthreads = pow((last_level - current_level), 2);
    int *partes = divide_em_partes((high - low), nthreads);

    DadosThreadQuickSort dt[nthreads];
    pthread_t threads[nthreads];

    int x, k = 0;
    for(x = 0; x < nthreads; x++){
        dt[x].low = k;
        dt[x].high = partes[x] + k;
        k = dt[x].high;
    }

    swap(&matrix[0][(*i) + 1], &matrix[0][high]);
    return (i + 1);
}

void *quickSort_paralelo(void *_dt)
{
    DadosThreadQuickSort *dt = (DadosThreadQuickSort*) _dt;
    int low = dt->low, high = dt->high;
    int current_level = dt->current_level, last_level = dt->last_level;
    double **matrix = dt->matrix;

    if (low < high)
    {
        if(current_level < last_level){
            int pi = partition_paralelo(matrix, low, high, current_level, last_level);

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
            int pi = partition(matrix, low, high);

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

