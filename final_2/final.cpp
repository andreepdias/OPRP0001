#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>
#include <pthread.h> 
#include <omp.h>
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 
#include <cmath>
#include <algorithm>
#include <vector>

#define ABC_SIZE 64
#define VERIFICAR 200

const char alfabeto[ABC_SIZE+1] = "./0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

using namespace std;

typedef struct
{
    int rank;
    pthread_cond_t *cond;
    pthread_mutex_t *lock;
    int numero_cifras;
    int numero_salts;
    int *inicio_salts;
    char *salt_encontrado;
    char *cifra_encontrada;
    char *quebrou_alguma_senha;
    char *terminou_execucao;
} DadosThread;


//Sincroniza as cifras e salts já encontrados
void *thread_sincronizacao(void *_dt)
{
    DadosThread *dt = (DadosThread *)_dt;
    
    int rank = (*dt).rank;

    pthread_cond_t *cond = (*dt).cond;
    pthread_mutex_t *lock = (*dt).lock;
    int numero_cifras = (*dt).numero_cifras;
    int numero_salts = (*dt).numero_salts;
    int *inicio_salts = (*dt).inicio_salts;
    char *salt_encontrado = (*dt).salt_encontrado;
    char *cifra_encontrada = (*dt).cifra_encontrada;
    char *quebrou_alguma_senha = (*dt).quebrou_alguma_senha;    
    char *terminou_execucao = (*dt).terminou_execucao;
    char cifra_encontrada_buffer[numero_cifras];
    char salt_encontrado_buffer[numero_salts];

    MPI_Status status;
    int flag, tag = 0;;

    while(!(*terminou_execucao)){
        pthread_cond_wait(cond, lock); 
        if(rank == 0){
            flag = false;
            MPI_Iprobe(MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &flag, &status);
            while(flag){
                MPI_Recv(cifra_encontrada_buffer, numero_cifras, MPI_CHAR, status.MPI_SOURCE, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                for(int n = 0; n < numero_cifras; n++) 
                {
                    cifra_encontrada[n] |= cifra_encontrada_buffer[n];
                }
                flag = false;
                MPI_Iprobe(MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &flag, &status);                    
            }
            // *** processa salts com todas cifras resolvidas 
            // bcast(salts_resolvidos -> buffer)

            //Encontra os salts já resolvidos
            for(int i = 0; i < numero_salts; i++){
                int j = inicio_salts[i];
                bool falta_alguem = false;
                while(j < inicio_salts[i+1]){
                    if(not cifra_encontrada){
                        falta_alguem = true;
                        break;
                    }
                }
                if(not falta_alguem) salt_encontrado[i] = 1;
            }


            MPI_Bcast(cifra_encontrada, numero_cifras, MPI_CHAR, 0, MPI_COMM_WORLD);
            MPI_Bcast(salt_encontrado, numero_salts, MPI_CHAR, 0, MPI_COMM_WORLD);
    
        }
        else{ //Só busca a sincronização
            if((*quebrou_alguma_senha)){
                memcpy(cifra_encontrada_buffer, cifra_encontrada, numero_cifras);
                MPI_Send(cifra_encontrada_buffer, numero_cifras, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
            }
            (*quebrou_alguma_senha) = false;
            MPI_Bcast(cifra_encontrada, numero_cifras, MPI_CHAR, 0, MPI_COMM_WORLD);
            MPI_Bcast(salt_encontrado, numero_salts, MPI_CHAR, 0, MPI_COMM_WORLD);

        }
    }
}


void number2word(long num, char* palavra);

int main(int argc, char ** argv){

    int *sndbuffer, recvbuffer;
    int rank, size, tag = 0; 
    long int i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    int tamanho_todas_cifras;

    char *cifras;
    vector<string> lista_cifras;

   if(rank == 0){
        ifstream file(argv[1]);
    
        string line;
        while (getline(file, line)){
            lista_cifras.push_back(line);
        }
        file.close();

        int numero_cifras = lista_cifras.size();
        int tamanho_cifra = lista_cifras[0].size();
        
        tamanho_todas_cifras = numero_cifras * tamanho_cifra + 1;
        
        cifras = new char[tamanho_todas_cifras];

        sort(lista_cifras.begin(), lista_cifras.end());

        for (int i = 0; i < numero_cifras; i++){
            for(int j = 0; j < tamanho_cifra; j++){
                cifras[i * tamanho_cifra + j] = lista_cifras[i][j];
            }
        }

        cifras[tamanho_todas_cifras] = '\0';
    }

    MPI_Bcast(&tamanho_todas_cifras, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank != 0){
        cifras = new char[tamanho_todas_cifras];
    }

    MPI_Bcast(cifras, tamanho_todas_cifras, MPI_CHAR, 0, MPI_COMM_WORLD);


    int numero_cifras = (tamanho_todas_cifras - 1) / 13; //Número de palavras codificas (cifras) a processar
    int tamanho_cifra = 13;

    char cifras_vet[numero_cifras][tamanho_cifra + 1];
    for(int i = 0; i < numero_cifras; i++){
        for(int j = 0; j < tamanho_cifra; j++){
            cifras_vet[i][j] = cifras[i * tamanho_cifra + j];
        }        
        cifras_vet[i][tamanho_cifra] = '\0';
    }

    int tamanho_maximo_palavra = 8; //Máximo tamanho possível da senha (resposta)
    
    char cifra_encontrada[numero_cifras];
    memset(cifra_encontrada, 0, numero_cifras);


    char palavra[9];
    char* cifra;
    char salt[3];

    int inicio_salts[numero_cifras];
    for(int i = 0; i < numero_cifras; i++){
        inicio_salts[i] = -1;
    }
    inicio_salts[0] = 0;
    

    //Encontra onde começa cada salt (numero da cifra);
    int n_salt = 1;
    cifra = cifras_vet[0];

    char salt_anterior[3];
    salt_anterior[0] = cifra[0];
    salt_anterior[1] = cifra[1];
    salt_anterior[2] = '\0';

    for(int i = 1; i < numero_cifras; i++){
        cifra = cifras_vet[i];
        salt[0] = cifra[0];
        salt[1] = cifra[1];
        salt[2] = '\0';
        if(strcmp(salt, salt_anterior) != 0){
            inicio_salts[n_salt] = i;
            n_salt++;
        }
        strcpy(salt_anterior, salt);
    }
    inicio_salts[n_salt] = numero_cifras; 
    
    char salt_encontrado[n_salt];
    memset(salt_encontrado, 0, n_salt);
    

    struct crypt_data cd[n_salt + 1];

    for(int i = 0; i < n_salt + 1; i++){
        cd[i].initialized = 0;
    }

    long int numero_possibilidades = 0;
    for(int i = 1 ; i <= tamanho_maximo_palavra; i++){
        numero_possibilidades += std::pow(ABC_SIZE, i); 
    }

    long end = numero_possibilidades;
    char cifra_encontrada_buffer[numero_cifras];

    char *quebrou_alguma_senha = new char;
    (*quebrou_alguma_senha) = false;

    
    /* Prepara Thread de Sincronização entre nós */
    pthread_cond_t *cond = new pthread_cond_t;
    (*cond) = PTHREAD_COND_INITIALIZER; 

    pthread_mutex_t *lock = new pthread_mutex_t;
    (*lock) = PTHREAD_MUTEX_INITIALIZER; 
    
    char *terminou_execucao = new char;
    (*terminou_execucao) = false;

    pthread_t thread;
    DadosThread dt;

    dt.rank = rank;
    dt.cond = cond;
    dt.lock = lock;
    dt.numero_cifras = numero_cifras;
    dt.numero_salts = n_salt;
    dt.inicio_salts = inicio_salts; 
    dt.cifra_encontrada = cifra_encontrada;
    dt.salt_encontrado = salt_encontrado;
    dt.quebrou_alguma_senha = quebrou_alguma_senha;
    dt.terminou_execucao = terminou_execucao;

    pthread_create(&thread, NULL, thread_sincronizacao, (void *)(&dt));
    


    int quantos_loops = 0; //contador pra ver se tem que verificar cifras encontradas (o numero de loops é define VERIFICAR)
    #pragma omp parallel for schedule(dynamic) shared(cifras, rank, size, end, tamanho_cifra, numero_cifras, inicio_salts, salt_encontrado, cifra_encontrada, cifra_encontrada_buffer, quantos_loops, status) private(palavra, cifra, cd)
    for(i = rank; i < end; i += size){

        number2word(i, palavra); // colocar INLINE
        int salt_atual = 0;

        while(inicio_salts[salt_atual + 1] != -1){
            if(salt_encontrado[salt_atual]) continue;
            //Percorre cifras de cada salt
            cifra = cifras_vet[inicio_salts[salt_atual]];

            const char* palavra_cifrada = crypt_r(palavra, cifra, &cd[salt_atual]); //cifra a palavra com o salt atual

            for(int cifra_atual = inicio_salts[salt_atual]; cifra_atual < inicio_salts[salt_atual + 1]; cifra_atual++){
                if(cifra_encontrada[cifra_atual]) continue;

                cifra = cifras_vet[cifra_atual];
                if(strcmp(cifra, palavra_cifrada) == 0){
                    (*quebrou_alguma_senha) = true;
                    cifra_encontrada[cifra_atual] = 1;
                    printf("Rank: %2d\tcifra(%3d): %s\tsenha: %s\n", rank, cifra_atual, cifra, palavra);
                }
            }
            // printf("Salt atual:%d\n", salt_atual);
            salt_atual++;

        }
        
        int tid = omp_get_thread_num();
        if(tid == 0){
            quantos_loops++;
            if(quantos_loops == VERIFICAR){
                quantos_loops = 0;
                pthread_cond_signal(cond); 
            }
        }
         
    }

    
    (*terminou_execucao) = true;

    pthread_join(thread, NULL);
    

    MPI_Finalize();

    return 0;
    

}

void number2word(long num, char *palavra)
{
    char palavra_inv[9];
    int cont = 0;

    // First figure out how many digits there are.
    num += 1; // This line is funky
    int c = 0;
    long x = 1;
    while (num >= x)
    {
        c++;
        num -= x;
        x *= ABC_SIZE;
    }

    // Now you can do normal base conversion.
    for (int i = 0; i < c; i++)
    {
        palavra_inv[i] = alfabeto[num % ABC_SIZE];
        num = num / ABC_SIZE;
    }
    palavra_inv[9] = '\0';

    for (int i = 0, j = c - 1; i < c; i++, j--)
    {
        palavra[i] = palavra_inv[j];
    }
    palavra[c] = '\0';
}

