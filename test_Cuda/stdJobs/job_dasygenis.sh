#!/bin/bash
#PBS -l nodes=1:ppn=1:gpus=1
#PBS -l walltime=00:30:00
#PBS -N NODES_4_PPN_24_DASYGENIS
#PBS -m n
#PBS -j oe

echo $HOSTNAME
cd /zstorage/home/csd2194/test_Cuda/
LD_LIBRARY_PATH=/usr/local/cuda-7.5/lib64 ./main







