#include <mpi.h>
#include <stdio.h>


int main(int argc, char **argv){

    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    printf("Helloo world from processor %s, rank %d out of %d processor\n", processor_name, world_rank, world_size);

    MPI_Finalize();

}

/*
scp -r 192.168.1.32:~/.ssh .

scp mpi.c 192.168.1.32:~/andre
mpirun -machinefile ../m.txt -np 80 a.out 
*/