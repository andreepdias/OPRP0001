#include "matrix.h"
#include "quicksort_threads.h"

void *trabalho_thread_quick_sort(void *_dt)
{ 
    DadosThreadQuickSort *dt = (DadosThreadQuickSort*) _dt;

    int low = dt->low;
    int high = dt->high;
    int current_level = dt->current_level;
    int last_level = dt->last_level;
    double **matrix;
    matrix = *(dt->matrix);

    int piece = (high - low + 1) / 2;
 
    printf("Current Level: %d\tLast Level:%d\nlow:%d\thigh:%d\npiece:%d\n\n", current_level, last_level, low, high, piece);


    if(current_level < last_level){

        DadosThreadQuickSort *dt = NULL;
        pthread_t *threads = NULL;

        if (!(dt = (DadosThreadQuickSort *) malloc(sizeof(DadosThreadQuickSort) * 2))) {
            printf("Erro ao alocar memória\n");
            exit(EXIT_FAILURE);
        }
        if (!(threads = (pthread_t *) malloc(sizeof(pthread_t) * 2))) {
            printf("Erro ao alocar memória\n");
                    exit(EXIT_FAILURE);
        }

        int i;
        for(i = 0; i < 2; i++){
            dt[i].id = pow(10, current_level) + (i + 1);
            dt[i].matrix = &matrix;
            dt[i].current_level = current_level + 1;
            dt[i].last_level = last_level;
        }
        dt[0].low = low;
        dt[0].high = piece - 1;
        dt[1].low = piece;
        dt[1].high = high;
        for(i = 0; i < 2; i++){
            pthread_create(&threads[i], NULL, trabalho_thread_quick_sort, (void *) (dt + i));
        }
        for(int i = 0; i < 2; i++){
            pthread_join(threads[i], NULL);		
        }
        quickSort(matrix, low, high);
    }else {
        quickSort(matrix, low, piece - 1);
        quickSort(matrix, piece, high);
    }

} 


matrix_t *matrix_sort_quick_paralelo(matrix_t *A, int nthreads)
{
    matrix_t *m = matrix_clone(A);

    int n = A->rows * A->cols;
    int depth = (int)log2(nthreads);

    DadosThreadQuickSort *dt = NULL;
    pthread_t *threads = NULL;

    if (!(dt = (DadosThreadQuickSort *) malloc(sizeof(DadosThreadQuickSort) * 1))) {
        printf("Erro ao alocar memória\n");
        exit(EXIT_FAILURE);
    }
    if (!(threads = (pthread_t *) malloc(sizeof(pthread_t) * 1))) {
        printf("Erro ao alocar memória\n");
                exit(EXIT_FAILURE);
    }

    dt[0].id = 1;
    dt[0].matrix = &(A->matrix);
    dt[0].current_level = 1;
    dt[0].last_level = depth;
    dt[0].low = 0;
    dt[0].high = n - 1;
    pthread_create(&threads[0], NULL, trabalho_thread_quick_sort, (void *) (dt + 0));
    pthread_join(threads[0], NULL);		

    quickSort(A->matrix, 0, n - 1);

    return m;
}
