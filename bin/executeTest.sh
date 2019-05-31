#!/bin/bash
echo "Insert max number of cores: "
read numProc
printf "\nExecuting sequential algorithm...\n"
./seq

if [ $numProc -gt 1 ]
then
    for((i=2;i<=numProc;i++))
    do
        printf "\nExecuting parallel algorithm with $i cores...\n"
        mpirun -np $i par
    done
fi