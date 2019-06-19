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


void number2word(long num, char* palavra);
string number2word(long num);


int main(int argc, char ** argv){

    int *sndbuffer, recvbuffer;
    int rank, size, tag = 0; 
    long int i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    printf("KKKKKKKKKKKKk\n");

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

    char cifras_vet[numero_cifras][tamanho_cifra+1];
    for(int i = 0; i < numero_cifras; i++){
        for(int j = 0; j < tamanho_cifra; j++){
            cifras_vet[i][j] = cifras[i*tamanho_cifra + j];
        }        
        cifras_vet[i][tamanho_cifra] = '\0';
    }

    int tamanho_maximo_palavra = 8; //Máximo tamanho possível da senha (resposta)
    
    char cifra_encontrada[numero_cifras];
    for(int i = 0; i < numero_cifras; i++) cifra_encontrada[i] = 0;
    
    char palavra[9];
    char* cifra;
    char salt[3];

    int inicio_salts[numero_cifras];
    for(int i = 0; i < numero_cifras; i++){
        inicio_salts[i] = -1;
    }
    // memset(inicio_salts, -1, numero_cifras-1);
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

    pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER; 
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 

    bool quebrou_alguma_senha = false;

    
    #pragma omp parallel
    {
        omp_set_nested(1);
        #pragma omp sections
        {
            #pragma omp section
            {
                pthread_cond_wait(&cond1, &lock); 
                if(rank == 0){ 
                    int flag = false;
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
                    MPI_Bcast(cifra_encontrada, numero_cifras, MPI_CHAR, 0, MPI_COMM_WORLD);

                }
                else{ //Só busca a sincronização
                    if(quebrou_alguma_senha){
                        memcpy(cifra_encontrada_buffer, cifra_encontrada, numero_cifras);
                        MPI_Send(cifra_encontrada_buffer, numero_cifras, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
                    }
                    quebrou_alguma_senha = false;
                    MPI_Bcast(cifra_encontrada, numero_cifras, MPI_CHAR, 0, MPI_COMM_WORLD);
                }
            }

            #pragma omp section
            {
                int quantos_loops = 0; //contador pra ver se tem que verificar cifras encontradas (o numero de loops é define VERIFICAR)
                #pragma omp parallel for schedule(dynamic) shared(cifras, rank, size, end, tamanho_cifra, numero_cifras, inicio_salts, cifra_encontrada, cifra_encontrada_buffer, quantos_loops, status) private(palavra, cifra, cd)
                for(i = rank; i < end; i += size){

                    number2word(i, palavra); // colocar INLINE
                    int salt_atual = 0;

                    while(inicio_salts[salt_atual + 1] != -1){
                        //Percorre cifras de cada salt
                        cifra = cifras_vet[inicio_salts[salt_atual]];

                        const char* palavra_cifrada = crypt_r(palavra, cifra, &cd[salt_atual]); //cifra a palavra com o salt atual

                        for(int cifra_atual = inicio_salts[salt_atual]; cifra_atual < inicio_salts[salt_atual + 1]; cifra_atual++){
                            if(cifra_encontrada[cifra_atual]) continue;

                            cifra = cifras_vet[cifra_atual];
                            if(strcmp(cifra, palavra_cifrada) == 0){
                                quebrou_alguma_senha = true;
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
                            pthread_cond_signal(&cond1); 
                        }
                    }
                }
            }
        }
    }



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

// void number2word(long num, char* palavra){
//     //14 (abc(4)abc(7))  = 4*64^1 + 7*64^0 = 263 
//     //263 = dividir sucessivamente, como de decimal para binário
//     // stringstream palavra_ss;
//     char palavra_inv[9];
//     int cont = 0;

//     ldiv_t temp;
//     temp.quot = num;

//     while(temp.quot != 0){
//         temp = ldiv(temp.quot, ABC_SIZE);
//         palavra_inv[cont] = alfabeto[temp.rem - 1];
//         cont++;
//     } 
//     palavra_inv[cont] = '\0';
//     // string palavra = palavra_ss.str();
//     // reverse(palavra.begin(), palavra.end());
//     //Inverte palavra
//     for(int i = 0, j = cont-1; i < cont+1; i++, j--){
//         palavra[i] = palavra_inv[j]; 
//     }
//     palavra[cont] = '\0';
// }

string number2word(long num){
    //14 (abc(4)abc(7))  = 4*64^1 + 7*64^0 = 263 
    //263 = dividir sucessivamente, como de decimal para binário
    stringstream palavra_ss;

    ldiv_t temp;
    temp.quot = num;

    while(temp.quot != 0){
        temp = ldiv(temp.quot, ABC_SIZE);
        palavra_ss << alfabeto[temp.rem - 1];
    } 
    string palavra = palavra_ss.str();
    reverse(palavra.begin(), palavra.end());

    return palavra;
}

