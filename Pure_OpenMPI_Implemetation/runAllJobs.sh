#!/bin/bash

cd stdJobs
for i in `seq 1 6`;
do
for j in `seq 1 24`;
do
filename=job$i$j.sh
touch $filename
echo "#!/bin/bash
#PBS -l nodes=${i}:ppn=${j}
#PBS -l walltime=00:2:00
#PBS -N NODES_${i}_PPN_${j}
#PBS -m n
#PBS -j oe

echo $HOSTNAME
cd /zstorage/home/csd2194/Pure_OpenMPI_Implemetation/
mpirun ./main 1920 1080 16 7 400 100 /mnt/videonas/1920x1080/BigBuckBunny_1920_1080_24fps.yuv">$filename
#    qsub $filename
#    sleep 5s
done
done

