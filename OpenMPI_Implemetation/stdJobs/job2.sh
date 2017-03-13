#!/bin/bash
#PBS -l nodes=5:ppn=16
#PBS -l walltime=00:30:00
#PBS -N NODES_5_PPN_16
#PBS -m n
#PBS -j oe

echo $HOSTNAME
cd /zstorage/home/csd2194/OpenMPI_Implemetation/
export OMP_NUM_THREADS=8
echo Omp Threads: $OMP_NUM_THREADS
mpirun ./main 1920 1080 16 7 400 100 /mnt/videonas/1920x1080/BigBuckBunny_1920_1080_24fps.yuv







