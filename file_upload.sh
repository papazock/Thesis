#!/bin/bash
#set -x

clear

file="$1"

echo "upload file to $file"

scp -i ~/.ssh/id_rsa /home/barboutara/ClionProjects/Thesis/"$file" csd2194@zafora.icte.uowm.gr:/zstorage/home/csd2194


