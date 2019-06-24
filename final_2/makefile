build:
	mpic++ final.cpp -O3 -lcrypt -fopenmp -pthread -o final
	sh scp_executable.sh ips.txt final

run:
	mpirun -np $(np) --map-by socket:pe=4 --machinefile ips.txt final input

kill:
	sh mata_processos.sh ips.txt final

mca:
	mpirun -np 2 -N 1 --mca btl_tcp_if_include enp2s0 --machinefile ips.txt final input
