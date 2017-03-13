# 2014/02
# Dasygenis multipurpose Makefile

#Override the compiler with:
#CC=clang;export CC

CUDA_PATH= /usr/local/cuda-7.5
#-pg is for profiling
PG=-DDEBUG=1
CC?=g++
CUDAC=$(CUDA_PATH)/bin/nvcc
CFLAGS=-O0 ${PG} -g -fopenmp -Wall
LDFLAGS=${PG} -lm -lc -lrt -fopenmp
SOURCES=
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

MPI_COMPILE_FLAGS = $(shell mpicc --showme:compile)
MPI_LINK_FLAGS = $(shell mpicc --showme:link)

CUDAROOT=/usr/local/cuda-7.5
hosts=83.212.19.248,83.212.19.247
CUDA=${CUDAROOT}/bin/nvcc
MPI=/usr/bin/mpicc
RUN=LD_LIBRARY_PATH=$(CUDAROOT)/lib64 mpirun
LIBS=-lcudart -L ${CUDAROOT}/lib64 -lm
INCLUDES=-I ${CUDAROOT}/include


all: $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) main.o
		$(CC) $(OBJECTS) main.o $(LDFLAGS) -o $@ $(LIBS) $(INCLUDES)


.c.o:
		$(CC) $(CFLAGS) -c $< -o $@

main.o: main.cu
		$(CUDA) -c main.cu -o main.o $(LIBS)

clean:
		rm -f main *.o gmon.out callgrind.out.* a.out core *.core *.clang *.out *.raw main_extract_frame


execute: ${EXECUTABLE}
		 LD_PRELOAD="$(CUDAROOT)/lib64/libcudart.so.7.5" `pwd`/$(EXECUTABLE)

local: ${EXECUTABLE}
		mpirun -np 10 --bysocket -bind-to-socket -report-bindings `pwd`/$(EXECUTABLE)


