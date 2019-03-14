#include "matrix.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int id;
	matrix_t *A;
	matrix_t *B;
	matrix_t *C;
	int l_inicio;
	int l_fim;
	// id % n_linhas
	// 
} DadosThread;


void *trabalho_thread_soma(void *);

int main(int argc, char *argv[])
{
	int num_threads = 0;
	int i, nrows, ncols;

	if (argc < 4) {
		printf("informe: %s <num_threads> <rows> <cols>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	num_threads = atoi(argv[1]);
	nrows = atoi(argv[2]);
	ncols = atoi(argv[3]);

	printf("num_threads = %d\tnrows = %d\tncols = %d\n", num_threads, nrows, ncols);

	DadosThread *dt = NULL;

	pthread_t *threads = NULL;

	if (!(dt = (DadosThread *) malloc(sizeof(DadosThread) * num_threads))) {
		printf("Erro ao alocar memória\n");
		exit(EXIT_FAILURE);
	}

	if (!(threads = (pthread_t *) malloc(sizeof(pthread_t) * num_threads))) {
		 printf("Erro ao alocar memória\n");
                exit(EXIT_FAILURE);
	}

	matrix_t *a = matrix_create(nrows, ncols);
    matrix_randfill(a);
	matrix_print(a);
	matrix_t *b = matrix_create(nrows, ncols);
    matrix_randfill(b);
	matrix_print(b);
	matrix_t *c = matrix_create(nrows, ncols);
	matrix_print(c);

	int k = 0;
	for (i = 0; i < num_threads; i++) {
		dt[i].id = i;
		dt->A = a;
		dt->B = b;
		dt->C = c;
		dt->l_inicio = k;
		dt->l_fim = nrows / num_threads + k;
		k = dt->l_fim;

		if(i == num_threads - 1 && k != nrows){
			dt->l_fim = nrows;
		}
		printf("l_inicio: %d\nl_fim:%d\nk: %d\n", dt->l_inicio, dt->l_fim, k);

		pthread_create(&threads[i], NULL, trabalho_thread_soma, (void *) (dt + i));
	}
	

	for (i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);		
	}

	free(dt);
	free(threads);

	matrix_print(c);
	return EXIT_SUCCESS;
}

void *trabalho_da_thread(void *arg)
{
	DadosThread *p = (DadosThread *) arg;
	
	printf("Thread %d executando\n", p->id);

	return NULL;
}

void *trabalho_thread_soma(void *arg)
{
	DadosThread *p = (DadosThread *) arg;
	printf("TRABALHANDO ID: %d\n", p->id);
	
	int i, j;
	for(i = p->l_inicio; i < p->l_fim; i++){
		for(j = 0; j < p->A->cols; j++){
			p->C->data[i][j] = p->A->data[i][j] + p->B->data[i][j];
			printf("id:%d\tsoma:%lf\ti:%d\tj:%d\n", p->id, p->C->data[i][j], i, j);
		}
	}

	printf("Thread %d executando\n", p->id);

	return NULL;
}
