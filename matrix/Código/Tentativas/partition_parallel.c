
typedef struct
{
    double **matrix;
    int low;
    int high;
    int pivot;
    int *i;
    pthread_mutex_t *mutex_i;
} DadosThreadPartition;


/* não melhorou :( */
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

/* não melhorou :( */
int partition_paralelo(double **matrix, int low, int high, int current_level, int last_level)
{
    int random = low + rand() % (high - low);
    swap(&matrix[0][random], &matrix[0][high]);

    double pivot = matrix[0][high];
    int *i = malloc(sizeof(int));
    (*i) = low - 1;

    int nthreads = pow((last_level - current_level), 2);
    int *partes = divide_em_partes((high - low), nthreads);

    DadosThreadPartition dt[nthreads];
    pthread_t threads[nthreads];

    pthread_mutex_t *mutex_i = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(mutex_i, NULL);

    int x, k = low;
    for(x = 0; x < nthreads; x++){
        dt[x].low = k;
        dt[x].high = partes[x] + k;
        k = dt[x].high;
        dt[x].pivot = pivot;
        dt[x].i = i;
        dt[x].matrix = matrix;
        dt[x].mutex_i = mutex_i;
        pthread_create(&threads[x], NULL, partition_swap, (void *)(dt + x));
    }
    for(x = 0; x < nthreads; x++){
        pthread_join(threads[x], NULL);
    }

    pthread_mutex_destroy(mutex_i);
    mutex_i = NULL;

    swap(&matrix[0][(*i) + 1], &matrix[0][high]);
    return ((*i) + 1);
}
