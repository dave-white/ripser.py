.PHONY = ripser ripser-debug ripser-omp

all: ripser ripser-debug ripser-omp

default: ripser

ripser:
	g++ -c ripser.cpp -o ripser.o

ripser-debug:
	g++ -c ripser.cpp -g3 -fopenmp -o ripser.o

ripser-omp:
	g++ -c ripser.cpp -DUSE_OMP -fopenmp -o ripser.o

