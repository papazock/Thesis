#!/bin/bash
#PBS -l nodes=1:gpus=1
#PBS -l walltime=00:30:00
#PBS -N CUDA_GPUS_1
#PBS -m n
#PBS -j oe

echo $HOSTNAME
cd /zstorage/home/csd2194/Cuda_Implemetation/
make
#LD_LIBRARY_PATH=/usr/local/cuda-7.5/lib64 /usr/local/cuda-7.5/bin/nvprof ./main 1920 1080 16 7 400 100 /mnt/videonas/1920x1080/BigBuckBunny_1920_1080_24fps.yuv
LD_LIBRARY_PATH=/usr/local/cuda-7.5/lib64 ./main 1920 1080 16 7 400 100 /mnt/videonas/1920x1080/BigBuckBunny_1920_1080_24fps.yuv
