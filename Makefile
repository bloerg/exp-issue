# Makefile for compilation of gcc and cuda version of pmuca ising2D

# please select your mpi compiler
#MPICC=mpic++
MPICC=/usr/lib64/openmpi/bin/mpic++
CPU_FLAGS=-pedantic -Wall -Wextra -O3 -std=c++0x -I ./include/ -I ./include/Random123/include/ -I ./include/Random123/examples/

ifeq ($(CONFIG),debug)
	OPT =-O0 -g
else
	OPT =
endif

all: exp_cl exp_cpp

exp_cl: test_exp_cl.cpp
	g++ test_exp_cl.cpp -lOpenCL $(CPU_FLAGS) $(OPT) -o $@

exp_cpp: test_exp.cpp
	g++ test_exp.cpp $(CPU_FLAGS) $(OPT) -o $@


clean:
	rm -f exp_cl
	rm -f exp_cpp
	rm -f *.csv