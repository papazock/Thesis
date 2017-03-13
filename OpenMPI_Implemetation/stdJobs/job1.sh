#!/bin/bash
#PBS -l nodes=4:ppn=24
#PBS -l walltime=00:30:00
#PBS -N NODES_4_PPN_24
#PBS -m n
#PBS -j oe

echo $HOSTNAME
cd /zstorage/home/csd2194/OpenMPI_Implemetation/
OMP_NUM_THREADS=4
export OMP_PROC_BIND=true
export OMP_PLACES=cores
echo Omp Threads: $OMP_NUM_THREADS
mpirun -npersocket 1 -bind-to-socket --report-bindings ./main 1920 1080 16 7 400 100 /mnt/videonas/1920x1080/BigBuckBunny_1920_1080_24fps.yuv







