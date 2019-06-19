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
#include <vector>
#include <algorithm>

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

    int tamanho_todas_cifras;

    // struct crypt_data cd;
    // cd.initialized = 0;

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

    int tamanho_maximo_palavra = 8; //Máximo tamanho possível da senha (resposta)
    
    
    char palavra[9];
    char cifra[14];
    char salt[3];



    long int numero_possibilidades = 0;
    for(int i = 1 ; i <= tamanho_maximo_palavra; i++){
        numero_possibilidades += std::pow(ABC_SIZE, i); 
    }

    /*
    long num_palavras_tamanho = std::pow(ABC_SIZE, tamanho_cifra);
    long num_palavras_tamanho_anterior = 0;
    if(tamanho_cifra > 1){
        for(int i = 1; i < tamanho_cifra; i++)
            num_palavras_tamanho_anterior += std::pow(ABC_SIZE, i);
    }
    long int rank_slice = num_palavras_tamanho / size;
    
    long int begin = num_palavras_tamanho_anterior + rank_slice * rank;
    long int end = num_palavras_tamanho_anterior + (rank_slice * rank) + rank_slice;
    */



    long end = numero_possibilidades;
    
    map<char*, struct crypt_data*> salts;


    // #pragma omp parallel for schedule(dynamic) shared(cifras, rank, size, end, tamanho_cifra, numero_cifras) private(salts, palavra, cifra, salt)
    for(i = rank; i < end; i += size){

        number2word(i, palavra); // colocar INLINE

        for(int cifra_atual = 0; cifra_atual < numero_cifras; cifra_atual++){
            
            memcpy(cifra, cifras + cifra_atual * tamanho_cifra, tamanho_cifra);
            cifra[13] = '\0';

            memcpy(salt, cifra, 2);
            salt[2] = '\0';

            struct crypt_data *c;

            // printf("palavra: %s\tcifra: %s\tsalt: %s\n", palavra, cifra,  salt);

            if(salts.find(salt) == salts.end()){
                c = new crypt_data();
                c->initialized = 0;

                salts[salt] = c;
            }else{
                c = salts[salt];
            }
            if(strcmp(crypt_r(palavra, cifra, c), cifra) == 0){
                printf("Rank: %2d\tcifra(%3d): %s\tsenha: %s\ti: %ld\n", rank, cifra_atual, cifra, palavra, i);
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

