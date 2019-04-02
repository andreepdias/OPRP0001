#include "quicksort_threads.h"
#include "quicksort.h"
#include "matrix_threads.h"


void *quickSort_paralelo(void *_dt)
{
    DadosThreadQuickSort *dt = (DadosThreadQuickSort*) _dt;
    int low = dt->low, high = dt->high;
    int nthreads = dt->nthreads;
    double **matrix = dt->matrix;

    if (low < high)
    {
        if(nthreads <= 1){
            quickSort(matrix, low, high);
        }else{
            int pi = partition(matrix, low, high);
            int n = (high - low + 1) / nthreads;

            DadosThreadQuickSort dt[2];
            pthread_t threads[2];
            
            int i;
            for (i = 0; i < 2; i++)
            {
                dt[i].matrix = matrix;
            }
            dt[0].low = low;
            dt[0].high = pi - 1;
            dt[1].low = pi + 1;
            dt[1].high = high;

            int sum = 0;
            for(i = 1; i < nthreads; i++){
                if(sum + n + low - 1 >= pi){
                    dt[0].nthreads = i;
                    dt[1].nthreads = (nthreads - i);
                    break;
                }
                sum += n;
            }
            if(i == nthreads){
                dt[0].nthreads == nthreads - 1;
                dt[1].nthreads = 1;
            }

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

        DadosThreadQuickSort dt[1];
        dt[0].matrix = A->matrix;
        dt[0].low = 0;
        dt[0].high = n - 1;
        dt[0].nthreads = nthreads;

        quickSort_paralelo((void *)(dt + 0));

        return A;
}

