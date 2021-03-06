#!/bin/bash

cd stdJobs
i=$1
for j in 1 2 4 8 16 24 ;
do
filename=job$i$j.sh
touch $filename
echo "#!/bin/bash
#PBS -l nodes=${i}:ppn=${j}
#PBS -l walltime=00:7:00
#PBS -N NODES_${i}_PPN_${j}
#PBS -m n
#PBS -j oe

echo $HOSTNAME
cd /zstorage/home/csd2194/OpenMPI_Implemetation/
OMP_NUM_THREADS=8
export OMP_NUM_THREADS=8
echo Omp Threads: $OMP_NUM_THREADS=8
mpirun -npersocket -bind-to-socket ./main 1920 1080 16 7 400 100 /mnt/videonas/1920x1080/BigBuckBunny_1920_1080_24fps.yuv">$filename
qsub $filename
sleep 10s
done


