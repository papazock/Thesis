#!/bin/bash

mkdir Jobs
cd Jobs
for i in `seq 1 16`;
do

    touch job$i.sh
    echo job$i.sh created
     echo "#!/bin/bash
      #PBS -k o
      #PBS -l nodes=$i:ppn=8
      #PBS -l walltime=30:00
      #PBS -N Threads_$i
      #PBS -j oe
    cd /zstorage/home/csd2194/OpenMPI_Implemetation/
export OMP_NUM_THREADS=8
    mpirun -np $i ./main 1920 1080 16 7 500 2 /mnt/videonas/1920x1080/BigBuckBunny_1920_1080_24fps.yuv">job$i.sh
    qsub job$i.sh
    sleep 5s
done

