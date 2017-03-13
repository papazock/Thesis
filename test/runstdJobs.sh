#!/bin/bash

cd stdJobs

for i in `seq 1 3`;
do
    qsub job$i.sh
    sleep 5s
done


