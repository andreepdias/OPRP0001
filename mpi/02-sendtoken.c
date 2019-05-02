#include <mpi.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char **argv){

    int rank, size;
    int tag = 0;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int s = ((rank + 1) >= size) ? 0 : (rank + 1);
    int r = ((rank - 1) < 0) ? (size - 1) : (rank - 1);

    int token;
    if(rank == 0){
        token = 0;
        printf("Processo %d enviando token %d\n", rank, token);
        MPI_Send(&token, 1, MPI_INT, s, tag, MPI_COMM_WORLD);
        MPI_Recv(&token, 1, MPI_INT, r, tag, MPI_COMM_WORLD, &status);
        printf("Processo %d recebeu token %d\n", rank, token);
    }else{
        MPI_Recv(&token, 1, MPI_INT, r, tag, MPI_COMM_WORLD, &status);
        printf("Processo %d recebeu token %d ", rank, token);
        token += rank;
        printf("enviou token %d\n", token);
        MPI_Send(&token, 1, MPI_INT, s, tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;

}

/*
scp -r 192.168.1.32:~/.ssh .

scp mpi.c 192.168.1.32:~/andre
mpirun -machinefile ../m.txt -np 80 a.out 

MPI_Send(msg, 20, MPI_CHAR, i, tag, MPI_COMM_WORLD);
            /*
            pointeiros
            numero de bytes
            tipo
            destino / slave
            caminho - tag de dados / controle
            grupo de comunicação (troca informação entre o grupo)
            */
