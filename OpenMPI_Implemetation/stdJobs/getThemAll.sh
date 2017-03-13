#!/bin/bash
#PBS -l nodes=4:ppn=24+2:ppn=8
#PBS -l walltime=00:7:00
#PBS -N GetThemAll
#PBS -m n
#PBS -j oe

echo pleiades.icte.uowm.gr
cd /zstorage/home/csd2194/OpenMPI_Implemetation/
OMP_NUM_THREADS=8
export OMP_NUM_THREADS=8
echo Omp Threads: =8
mpirun -npersocket -bind-to-socket ./main 1920 1080 16 7 400 100 /mnt/videonas/1920x1080/BigBuckBunny_1920_1080_24fps.yuv







