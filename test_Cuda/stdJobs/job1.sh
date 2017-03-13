#!/bin/bash
      #PBS -k o
      #PBS -l nodes=4:ppn=24
      #PBS -l walltime=30:00
      #PBS -N NODES_4_PPN_24
      #PBS -j oe
    cd /zstorage/home/csd2194/test/
    mpirun -np 4 ./main
