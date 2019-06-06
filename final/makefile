build:
	mpic++ final.cpp -lcrypt -O3 -o final

run:
	mpirun -np $(1) --machinefile ips.txt final input

kill:
	sh mata_processos.sh ips.txt final

