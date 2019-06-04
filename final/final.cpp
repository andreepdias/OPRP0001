#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <crypt.h>

#define ABC_SIZE 64

const char alfabeto[65] = "./0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

using namespace std;

string number2word(long num);

int main(int argc, char ** argv){

    int *sndbuffer, recvbuffer;
    int rank, size, tag = 0;
    long long int i;

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


    long long int numero_possibilidades = 0;
    for(tamanho_cifra = 1 ; tamanho_cifra <= tamanho_maximo_cifra; tamanho_cifra++){
        numero_possibilidades += std::pow(ABC_SIZE, tamanho_cifra); 
    }


    long long int rank_slice = numero_possibilidades / size; //quanto cada rank vai processar

    if(numero_possibilidades % size != 0){
        printf("Sobrou %ld\n", numero_possibilidades % size);
    }

    while(numero_cifras--){
        const char* cifra = cifras_str.substr(cifra_atual * 13, 13).c_str();

        for(i = rank_slice * rank; i < (rank_slice * rank) + rank_slice; i++){

            const char* palavra = number2word(i).c_str(); //Talvez seja bom colocar a função inline depois

            if(strcmp(crypt(palavra, cifra), cifra) == 0){
                
                cout << "Rank " << rank << " cifra:" << cifra_atual << " senha:" << string(palavra) << endl;
                
                for(int j = 0; j < size; j++){
                    if(j != rank)
                        MPI_Send(&buffer, 1, MPI_INT, j, tag, MPI_COMM_WORLD);
                }

                // MPI_Bcast(palavra, strlen(palavra)+1, MPI_CHAR, rank, MPI_COMM_WORLD);
                break;

            }
            // MPI_Bcast(buffer, 9, MPI_CHAR, MPI_ANY_SOURCE, MPI_COMM_WORLD);
            // break;
            MPI_Iprobe(MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &flag, &status);
            if(flag){
                MPI_Recv(&buffer, 1, MPI_INT, status.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                // cout << "Rank estou aqui " << rank << endl; 
                flag = false;
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

