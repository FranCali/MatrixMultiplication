#!/bin/bash
echo "How many instances?"
read numInstances

echo "SCP started..."

cut -d' ' -f1 hfile | while read ip 
do
    scp -r bin pcpc@$ip:~
done

echo "SCP terminated"