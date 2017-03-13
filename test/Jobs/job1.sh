#!/bin/bash
      #PBS -k o
      #PBS -l nodes=4:ppn=24
      #PBS -l walltime=30:00
      #PBS -N Threads_1
      #PBS -j oe
    cd /zstorage/home/csd2194/OpenMPI_Implemetation/
export OMP_NUM_THREADS=8
    mpirun -np 4 ./main 1920 1080 16 7 500 2 /mnt/videonas/1920x1080/BigBuckBunny_1920_1080_24fps.yuv
