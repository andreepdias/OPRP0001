#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string> 
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

int main(int argc, char ** argv){

    int *sndbuffer, recvbuffer;
    int rank, size, i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int cifrassize;

    char *cifras;

   if(rank == 0){
        stringstream ss;

        ifstream file("input");
        
        string line;
        while (getline(file, line)){
            ss << line;
        }
        cout << ss.str();
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

    char alfabeto[65] = "./0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    int numero_cifras = (cifrassize - 1) / 13;
    int cifra_atual = 0;
    
    int tamanho_cifra = 1;
    int tamanho_maximo_cifra = 8;
    

    while(numero_cifras--){

        long numero_possibilidades = pow(64, tamanho_cifra);

    }


    MPI_Finalize();

    return 0;
    

}
