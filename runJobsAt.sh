#!/bin/bash
#set -X

cd $1

make clean
make

./runAllJobs.sh

cd

ls -l

echo "run ExtractTime to compare"