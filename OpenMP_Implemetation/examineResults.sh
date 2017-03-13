#!/bin/bash

for i in `seq 1 24`;
do
touch "EXAMINED_CORES_${i}.txt"
for j in `seq 1 24`;
do
    for tFile in "$(ls CORES_${i}_THREADS_${j}.*)";
    do
            time="$(grep Time $tFile)"
            echo $time
            echo "$time" >>EXAMINED_CORES_${i}.txt
    done

done
done