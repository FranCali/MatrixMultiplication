#!/bin/bash

echo "SSH broadcast started..."

read ip #Jump the first row localhost
cut -d' ' -f1 hfile | while read ip 
do
    ssh pcpc@$ip
    sleep 3
    exit
done

echo "SSH broadcast terminated"