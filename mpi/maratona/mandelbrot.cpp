/*
mpic++ mandelbrot.cpp -o andre
./scp_executable.sh andre
mpirun --mca btl_tcp_if_include enp2s0 -np 10 -machinefile ips.txt andre 23 79 240
*/
#include <complex>
#include <iostream>
#include <omp.h>
#include <mpi.h>

using namespace std;

int main(int argc, char **argv){
	
	int rank, size, tag = 0;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int max_row = stoi(argv[1]);
	int max_column = stoi(argv[2]);
	int max_n = stoi(argv[3]);
	/* 23 79 240 */

	int interval = ceil(double(max_row) / size);
	int begin = rank * interval;
	int end = begin + interval;
	
	if(rank == size - 1){
		end = max_row;
	}

	char *temp = (char*)malloc(sizeof(char) * (interval * size) * max_column);
	char **mat = (char**)malloc(sizeof(char*) * max_row);
	for (int i = 0; i < max_row; i++){
		mat[i] = &temp[i * max_column];
	}

	// printf("Rank[%d]\tbegin: %d\tend:%d\n", rank, begin, end);

	for(int r = begin; r < end; ++r){
		for(int c = 0; c < max_column; ++c){
			// printf("Rank[%d] r: %d c: %d\n", rank, r, c);
			// fflush(stdout);	
			complex<float> z;
			int n = 0;
			while(abs(z) < 2 && ++n < max_n)
				z = pow(z, 2) + decltype(z)(
					(float)c * 2 / max_column - 1.5,
					(float)r * 2 / max_row - 1
				);
			mat[r][c]=(n == max_n ? '#' : '.');
		}
	}

	// printf("Rank: %d\tBlock: %d\tMax: %d\tMaxMax: %d\n", rank, interval * max_column, max_row * max_column, interval * size * max_column);
	MPI_Gather((temp + (rank * interval * max_column)), interval * max_column, MPI_CHAR, (temp + (rank * interval * max_column)), interval * max_column, MPI_CHAR, 0, MPI_COMM_WORLD);

	if(rank == 0){
		for(int r = 0; r < max_row; ++r){
			for(int c = 0; c < max_column; ++c)
				std::cout << mat[r][c];
			cout << '\n';
		}	
		// printf("22 76: %c\n", mat[22][76]);
	}

	MPI_Finalize();
    
}

/*
	char **mat = (char**)malloc(sizeof(char*) * max_row);
	for (int i = 0; i < max_row; i++)
		mat[i] = (char*)malloc(sizeof(char)*max_column);
    */

/*
    MPI_Bcast(&max_row, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&max_column, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&max_n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	*

	if(rank == 0){
		for(int i = 1; i < size; i++){
			MPI_Send(&max_row, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
			MPI_Send(&max_column, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
			MPI_Send(&max_n, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
		}
	}else{
		MPI_Recv(&max_row, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		MPI_Recv(&max_column, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		MPI_Recv(&max_n, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
	}
	
	printf("(%d) - Received %d\n", rank, max_row);
	printf("(%d) - Received %d\n", rank, max_column);
	printf("(%d) - Received %d\n", rank, max_n);
 
 	cin >> max_row;
	cin >> max_column;
	cin >> max_n;
	*/

	/* Preciso informas as dimensões para todos
		for para trabalhadores */
	/*
		*/