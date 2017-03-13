#!/bin/bash
      #PBS -k o
      #PBS -l nodes=5:ppn=16
      #PBS -l walltime=30:00
      #PBS -N NODES_5_PPN_16
      #PBS -j oe
    cd /zstorage/home/csd2194/test/
    mpirun -np 5 ./main
