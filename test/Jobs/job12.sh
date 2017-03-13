#!/bin/bash
      #PBS -k o
      #PBS -l nodes=1:ppn=12
      #PBS -l walltime=30:00
      #PBS -N Threads_
      #PBS -j oe
    cd /zstorage/home/csd2194/OpenMP_Implemetation/
    ./main 1920 1080 16 7 500 2 /mnt/videonas/1920x1080/BigBuckBunny_1920_1080_24fps.yuv
#!/bin/bash
      #PBS -k o
      #PBS -l nodes=1:ppn=12
      #PBS -l walltime=30:00
      #PBS -N Threads_12
      #PBS -j oe
    cd /zstorage/home/csd2194/OpenMP_Implemetation/
    ./main 1920 1080 16 7 500 2 /mnt/videonas/1920x1080/BigBuckBunny_1920_1080_24fps.yuv
