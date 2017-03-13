#!/bin/bash
#PBS -l nodes=1:ppn=2:gpus=1
#PBS -l walltime=00:03:00
#PBS -N NODES_1_PPN_2_GPUS_2
#PBS -m n
#PBS -j oe


cd /zstorage/home/csd2194/MPI_Cuda_Implemetation/
OMP_NUM_THREADS=8
export OMP_NUM_THREADS=8
echo Omp Threads: =8
LD_LIBRARY_PATH=/usr/local/cuda-7.5/lib64 mpirun ./main 1920 1080 16 7 400 100 /mnt/videonas/1920x1080/BigBuckBunny_1920_1080_24fps.yuv