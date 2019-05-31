#!/bin/bash
echo "How many instances?"
read numInstances
echo "Enter the key file name: "
read keyfile

for ((i=0; i<$numInstances; i++))
do
    read ip
    ssh -i "$keyfile" pcpc@$ip
    sleep 2
    exit
done