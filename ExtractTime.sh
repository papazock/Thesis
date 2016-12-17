#!/bin/bash

# $1 filename pattern
# $2 output filename
# $3 delete files after extraction
if [ -z "$1" ]; then
    echo "first arg is empty, please provide the file name patter to Extract Time"
    exit 1
fi
if [ -z "$2" ]; then
    echo "second arg is empty, please provide the file name of the file to extract data"
    exit 1
fi

touch "$2"
list=$(ls $1*)
for i in $list
do
    tNum="$(echo $i | cut -d'_' -f 2 | cut -d'.' -f 1)"
    txt="$(echo Number of Threads $tNum)"
    time="$(grep Time $i)"
    echo "$txt" "$time" >>"$2"
done

if [ -n "$3" ]; then
    rm $1*
fi
    
nano $2