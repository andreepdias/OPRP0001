#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>
#include <pthread.h> 
#include <omp.h>
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


int main(int argc, char ** argv){

    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("%d %d\n", rank, size);

    MPI_Finalize();

}
