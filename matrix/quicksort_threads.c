#include "quicksort_threads.h"
#include "quicksort.h"


void *trabalho_thread_quick_sort(void *_dt)
{ 

    DadosThreadQuickSort *dt = (DadosThreadQuickSort*) _dt;

    int id = dt->id;
    int low = dt->low;
    int high = dt->high;
    int current_level = dt->current_level;
    int last_level = dt->last_level;
    int n = dt->n;
    double **matrix;
    matrix = *(dt->matrix);

    int piece = (high - low + 1) / 2;
    // printf("Id: %d\nCurrent Level: %d\tLast Level:%d\nlow:%d\thigh:%d\npiece:%d\n\n", id, current_level, last_level, low, high, piece);


    if(current_level < last_level && current_level < 3){

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
            dt[i].id = pow(10, current_level) + i;
            dt[i].matrix = &matrix;
            dt[i].current_level = current_level + 1;
            dt[i].last_level = last_level;
            dt[i].n = n;
        }
        if(current_level == 1){
            dt[0].id = dt[0].id + low + high;
            dt[1].id = dt[0].id + low + high;
        }
        dt[0].low = low;
        dt[0].high = piece - 1 + low;
        dt[1].low = piece + low;
        dt[1].high = high;
        for(i = 0; i < 2; i++){
            pthread_create(&threads[i], NULL, trabalho_thread_quick_sort, (void *) (dt + i));
        }
        for(int i = 0; i < 2; i++){
            pthread_join(threads[i], NULL);		
        }

        free(dt);
        free(threads);

        quickSort(matrix, low, high);
        
        /*
        if(id == 0){
            printf("**********************************\nlb1:%d\tub1:%d\n", low, high);
            int i;
            for(i = low; i <= high; i++){
                printf("%.4lf   ", matrix[0][i]);
            }
            printf("\n");
        }
        */
        

    }else {
        quickSort(matrix, low, piece - 1 + low);
        quickSort(matrix, piece + low, high);

        /*
        if(id == 56){
            printf("**********************************\nlb1:%d\tub1:%d\nlb2:%d\tub2:%d\n", low, piece - 1 + low, piece + low, high);
            int i;
            for(i = low; i <= piece - 1 + low; i++){
                printf("%.4lf   ", matrix[0][i]);
            }
            printf("\n\n");
            for(i = piece + low; i <= high; i++){
                printf("%.4lf   ", matrix[0][i]);
            }
            printf("\n\n");
        }
        */
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

    // printf("INICIANDO: Current: %d\tLast: %d\n", 1, depth);

    dt[0].id = 0;
    dt[0].matrix = &(m->matrix);
    dt[0].current_level = 0;
    dt[0].last_level = depth;
    dt[0].low = 0;
    dt[0].high = n - 1;
    dt[0].n = n / A->cols;
    pthread_create(&threads[0], NULL, trabalho_thread_quick_sort, (void *) (dt + 0));
    pthread_join(threads[0], NULL);

    free(dt);
    free(threads);

    quickSort(A->matrix, 0, n - 1);

    return m;
}
