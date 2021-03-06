#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string> 
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <crypt.h>
#include <atomic>
#include <omp.h>

#define ABC_SIZE 64

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

    int cifrassize;


    char *cifras;

   if(rank == 0){
        stringstream ss;

        ifstream file(argv[1]);
        
        string line;
        while (getline(file, line)){
            ss << line;
        }
        // cout << ss.str();
        file.close();

        cifrassize = ss.str().size() + 1;
        cifras = new char[cifrassize];
        strcpy(cifras, ss.str().c_str());
    }

    MPI_Bcast(&cifrassize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank != 0){
        cifras = new char[cifrassize];
    }

    MPI_Bcast(cifras, cifrassize, MPI_CHAR, 0, MPI_COMM_WORLD);

/* ----------------------------------- */

    string cifras_str = string(cifras);

    int numero_cifras = (cifrassize - 1) / 13; //Número de palavras codificas (cifras) a processar
    int cifra_atual = 0; //Em qual cifra estamos no momento
    
    int tamanho_cifra = 1; //Tamanho de senha testado no momento
    int tamanho_maximo_cifra = 8; //Máximo tamanho possível da senha (resposta)
    
    MPI_Status status;
    int flag = false;
    // char* buffer = (char*) malloc(sizeof(char)*9);
    int buffer = 0;
    char palavra[9]; //8 chars (no máximo) + \0

    bool decifrou_palavra;

    /*
    long int numero_possibilidades = 0;
    for(tamanho_cifra = 1 ; tamanho_cifra <= tamanho_maximo_cifra; tamanho_cifra++){
        numero_possibilidades += std::pow(ABC_SIZE, tamanho_cifra); 
    }

    long int rank_slice = numero_possibilidades / size; //quanto cada rank vai processar

    if(numero_possibilidades % size != 0){
        printf("Sobrou %ld\n", numero_possibilidades % size);
    }
    */

    while(numero_cifras--){

        for(tamanho_cifra = 1; tamanho_cifra <= tamanho_maximo_cifra; tamanho_cifra++){

            long num_palavras_tamanho = std::pow(ABC_SIZE, tamanho_cifra);

            long num_palavras_tamanho_anterior = 0;
            if(tamanho_cifra > 1){
                for(int i = 1; i < tamanho_cifra; i++)
                    num_palavras_tamanho_anterior += std::pow(ABC_SIZE, i);
            }

            long int rank_slice = num_palavras_tamanho / size;


            long int begin = num_palavras_tamanho_anterior + rank_slice * rank;
            long int end = num_palavras_tamanho_anterior + (rank_slice * rank) + rank_slice;

            // if(rank == 0){
            //      printf("#ZERO# ABC_SIZE: %d\ttamanho_cifra: %d\n#ZERO# rank_slice: %ld\tnum_palavras_tamanho: %ld\tsize: %d\tbegin: %ld\tend: %ld\n", ABC_SIZE, tamanho_cifra, rank_slice, num_palavras_tamanho, size, begin, end);
            // }

            if(rank == size - 1){
                end = num_palavras_tamanho;
            }

            const long N = end - begin;
            atomic<bool> go(false);
            long give = begin;

            #pragma omp parallel private(palavra)
            {
                struct crypt_data cd;

                int id = omp_get_thread_num();
                long i, stop;

                #pragma omp critical
                {
                    i = give;
                    give += N / omp_get_num_threads();
                    stop = give;

                    if (omp_get_thread_num() == omp_get_num_threads() - 1)
                        stop = end;
                }

                while (i < stop && !go)
                {
                    const char *cifra = cifras_str.substr(cifra_atual * 13, 13).c_str();

                    number2word(i, palavra); //Talvez seja bom colocar a função inline depois
                    // printf("id: %d\ti: %s\t%s\n", id, palavra, cifra);

                    if (strcmp(crypt_r(palavra, cifra, &cd), cifra) == 0)
                    {
                        printf("Rank: %2d\tcifra: %3d\tsenha: %s\ti: %15ld\tbegin: %15ld\tend: %15ld\n", rank, cifra_atual, palavra, i, begin, end);
                        go = true;
                        for (int j = 0; j < size; j++)
                        {
                            if (j != rank)
                                MPI_Send(&buffer, 1, MPI_INT, j, tag, MPI_COMM_WORLD);
                        }

                    }
                    if (id == 0)
                    {
                        MPI_Iprobe(MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &flag, &status);
                        if (flag)
                        {
                            go = true;
                            MPI_Recv(&buffer, 1, MPI_INT, status.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                            flag = false;
                        }
                    }
                    i++;
                }
            }
            if(go){
                break;
            }

        }
        // cout << "Pre-Barrier: Rank " << rank << " cifra:" << cifra_atual << endl;
        MPI_Barrier(MPI_COMM_WORLD);
        // cout << "Pos-Barrier: Rank " << rank << " cifra:" << cifra_atual << endl;
        cifra_atual++;
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

