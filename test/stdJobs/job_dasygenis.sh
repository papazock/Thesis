#!/bin/bash
#PBS -l nodes=4:ppn=24
#PBS -l walltime=00:30:00
#PBS -N NODES_4_PPN_24_DASYGENIS
#PBS -m n
#PBS -j oe

echo $HOSTNAME
cd /zstorage/home/csd2194/test/
mpirun ./hello_mpi_mp







