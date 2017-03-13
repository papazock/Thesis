#!/bin/bash
mkdir Jobs
cd Jobs

i=1
    for j in `seq 1 24`;
    do
    touch job$i$j.sh
    echo "#!/bin/bash
#PBS -l nodes=1:ppn=$j
#PBS -l walltime=00:04:00
#PBS -N CORES_${j}_THREADS_\${PBS_ARRAYID}
#PBS -m n
#PBS -j oe
cd /zstorage/home/csd2194/OpenMP_Implemetation/
export OMP_NUM_THREADS=\${PBS_ARRAYID}
./main 1920 1080 16 7 400 100 /mnt/videonas/1920x1080/BigBuckBunny_1920_1080_24fps.yuv">job$i$j.sh

    qsub -t 1-5%2 job$i$j.sh
    sleep 600s
    qsub -t 6-10%2 job$i$j.sh
    sleep 500s
    qsub -t 11-15%2 job$i$j.sh
    sleep 500s
    qsub -t 16-20%2 job$i$j.sh
    sleep 500s
    qsub -t 21-24%2 job$i$j.sh
    sleep 500s



    done