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

/* 23 79 240 */
int main(int argc, char **argv)
{

	int rank, size, tag = 0;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int max_row = stoi(argv[1]);
	int max_column = stoi(argv[2]);
	int max_n = stoi(argv[3]);

	int interval = ceil(double(max_row) / size);
	int begin = rank * interval;
	int end = begin + interval;

	char *temp = new char[interval * size * max_column];
	char **mat = new char *[max_row];
	// char *temp = (char *)malloc(sizeof(char) * (interval * size) * max_column);
	// char **mat = (char **)malloc(sizeof(char *) * max_row);
	for (int i = 0; i < max_row; i++)
	{
		mat[i] = &temp[i * max_column];
	}

	end = ((end + 1) >= max_row) ? max_row : end + 1;
	for (int r = begin; r < end; ++r)
	{
		for (int c = 0; c < max_column; ++c)
		{
			complex<float> z;
			int n = 0;
			while (abs(z) < 2 && ++n < max_n)
				z = pow(z, 2) + decltype(z)(
									(float)c * 2 / max_column - 1.5,
									(float)r * 2 / max_row - 1);
			mat[r][c] = (n == max_n ? '#' : '.');
		}
	}

	int block = interval * max_column;
	MPI_Gather((temp + (rank * block)), block, MPI_CHAR, (temp + (rank * block)), block, MPI_CHAR, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		for (int r = 0; r < max_row; ++r)
		{
			for (int c = 0; c < max_column; ++c)
				std::cout << mat[r][c];
			cout << '\n';
		}
	}

	MPI_Finalize();
}
