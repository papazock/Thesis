# 2014/02
# Dasygenis multipurpose Makefile

#Override the compiler with:
#CC=clang;export CC


#-pg is for profiling
PG=-DDEBUG=1
CC=mpicc
CFLAGS=-O0 ${PG} -g -fopenmp -Wall
LDFLAGS=${PG} -lm -lc -lrt -fopenmp
SOURCES=snr.c read_write_frame.c motion_compensation.c 3slog.c main.c dastools.c 3slogFRAME.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

CUDAROOT=/usr/local/cuda-7.5
CUDA=${CUDAROOT}/bin/nvcc
MPI=/usr/bin/mpicc
RUN=LD_LIBRARY_PATH=$(CUDAROOT)/lib64 mpirun
LIBS=-lcudart -L ${CUDAROOT}/lib64 -lm -g -pg
INCLUDES=-I ${CUDAROOT}/include


MPI_COMPILE_FLAGS = $(shell mpicc --showme:compile)
MPI_LINK_FLAGS = $(shell mpicc --showme:link)

all: $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 3slog_cuda.o
		$(CC) $(OBJECTS) 3slog_cuda.o $(MPI_LINK_FLAGS) $(LDFLAGS) -o $@ $(LIBS) $(INCLUDES)


.c.o:
		$(CC) $(MPI_COMPILE_FLAGS) $(CFLAGS) -c $< -o $@ $(LIBS) $(INCLUDES)

3slog_cuda.o: 3slog_cuda.cu
		$(CUDA) -c 3slog_cuda.cu -o 3slog_cuda.o $(LIBS) $(INCLUDES)

clean:
		rm -f main *.o gmon.out callgrind.out.* a.out core *.core *.clang *.out *.raw main_extract_frame

execute: ${EXECUTABLE}
		LD_PRELOAD="$(CUDAROOT)/lib64/libcudart.so.7.5" gdb `pwd`/$(EXECUTABLE) 1920 1080 16 7 500 2 /mnt/videonas/1920x1080/BigBuckBunny_1920_1080_24fps.yuv

debbug: ${EXECUTABLE}
		LD_PRELOAD="$(CUDAROOT)/lib64/libcudart.so.7.5" gdb ./$(EXECUTABLE)