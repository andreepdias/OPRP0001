#include "matrix.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int id;
} DadosThread;
// dar informações a uma thread
// pedaço da matriz, ponteiro


void *trabalho_da_thread(void *);
// toda thread no posix fará um trabalho com essa assinatura

int main(int argc, char *argv[])
{
	int num_threads = 0;	
	int i;

	if (argc < 2) {
		printf("informe: %s <num_threads>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	num_threads = atoi(argv[1]);

	printf("num_threads = %d\n", num_threads);

	DadosThread *dt = NULL;
	// malloc de 4 posições de dados da threads

	pthread_t *threads = NULL;
	// vetor nativo de threads (posição de memória onde existem 4 ponteiros para as threads)

	if (!(dt = (DadosThread *) malloc(sizeof(DadosThread) * num_threads))) {
		printf("Erro ao alocar memória\n");
		exit(EXIT_FAILURE);
	}

	if (!(threads = (pthread_t *) malloc(sizeof(pthread_t) * num_threads))) {
		 printf("Erro ao alocar memória\n");
                exit(EXIT_FAILURE);
	}

	for (i = 0; i < num_threads; i++) {
		dt[i].id = i;		
		pthread_create(&threads[i], NULL, trabalho_da_thread, (void *) (dt + i));
	}
	// para cada uma das 4 threads alimentem a informação do vetor dados
	// cria uma thread nova pasando o ponteiro , NULL = escalonamento, função que ela tem que fazer, dados que ela tem que manipular

	for (i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);		
	}
	// espera as threads terminarem

	free(dt);
	free(threads);
	return EXIT_SUCCESS;
}

void *trabalho_da_thread(void *arg)
{
	DadosThread *p = (DadosThread *) arg;
	
	printf("Thread %d executando\n", p->id);

	return NULL;
}
// entrou, converteu seu ponteiro e imprimiu
