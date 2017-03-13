# 2014/02
# Dasygenis multipurpose Makefile

#Override the compiler with:
#CC=clang;export CC


#-pg is for profiling
PG=-DDEBUG=1
CC?=mpicc
CFLAGS=-O0 ${PG} -g -fopenmp -Wall
LDFLAGS=${PG} -lm -lc -lrt -fopenmp
SOURCES=snr.c read_write_frame.c motion_compensation.c 3slog.c main.c dastools.c 3slogFRAME.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

MPI_COMPILE_FLAGS = $(shell mpicc --showme:compile)
MPI_LINK_FLAGS = $(shell mpicc --showme:link)


all: $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
		$(CC) $(OBJECTS) $(MPI_LINK_FLAGS) $(LDFLAGS) -o $@


.c.o:
		$(CC) $(MPI_COMPILE_FLAGS) $(CFLAGS) -c $< -o $@

clean:
		rm -f main *.o gmon.out callgrind.out.* a.out core *.core *.clang *.out *.raw main_extract_frame

execute: ${EXECUTABLE}
		`pwd`/$(EXECUTABLE) 1920 1080 16 7 500 2 /mnt/videonas/1920x1080/BigBuckBunny_1920_1080_24fps.yuv

local: ${EXECUTABLE}
		mpirun -np 10 `pwd`/$(EXECUTABLE) 1920 1080 16 7 500 2 /home/barboutara/ClionProjects/Thesis/test.yuv


