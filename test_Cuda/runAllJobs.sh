#!/bin/bash

mkdir Jobs
cd Jobs
for i in `seq 1 6`;
do

    touch job$i.sh
    echo job$i.sh created
     echo "#!/bin/bash
      #PBS -k o
      #PBS -l nodes=$i:ppn=8
      #PBS -l walltime=30:00
      #PBS -N Threads_$i
      #PBS -j oe
    cd /zstorage/home/csd2194/test/
    export OMP_NUM_THREADS=8
    mpirun ./main">job$i.sh
    qsub job$i.sh
    sleep 5s
done


