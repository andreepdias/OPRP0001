build:
	mpic++ final.cpp -lcrypt -O3 -o final -fopenmp -std=c++11
	sh scp_executable.sh ips.txt final

run:
	mpirun -np $(np) --machinefile ips.txt final input

kill:
	sh mata_processos.sh ips.txt final
