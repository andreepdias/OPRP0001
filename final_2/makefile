build:
	mpic++ final.cpp -O3 -lcrypt -fopenmp -pthread -o final
	sh scp_executable.sh ips.txt final

run:
	mpirun -np $(np) --map-by socket:pe=4 --machinefile ips.txt final input

kill:
	sh mata_processos.sh ips.txt final
