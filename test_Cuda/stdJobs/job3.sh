#!/bin/bash
      #PBS -k o
      #PBS -l nodes=6:ppn=2
      #PBS -l walltime=30:00
      #PBS -N NODES_6_PPN_2
      #PBS -j oe
    cd /zstorage/home/csd2194/test/
    mpirun -np 6 ./main
