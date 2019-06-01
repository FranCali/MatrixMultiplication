# MatrixMultiplication

The project contains both the sequential and parallel version of the matrix multiplication algorithm.<br>
The aim of this work is to compare both the parallel and the sequential algorithms for matrix multiplication and explain, using parallel computing parameters, the difference in performance and the benefits (if any) of the parallel solution.<br>
The algorithms are both written in C programming language. 
The parallel algorithm was developed using the Open MPI library.
The solution works with every type of matrices pair A,B (with the requirement that the number of columns of matrix A must be equal to the number of rows of matrix B). Input matrices can be both square and not square, moreover matrices rows number and/or columns number can be both divisible and not divisible by the number of processors.

## Parallel solution description
The parallel solution developed makes use of files for input and output. The input is read from file "infile" and the result output product matrix is written in file "outfile". Given p processing units and two matrices A:m x n and B: n x l in input, the algorithm applies the following work division strategy:
* Master node computes the total number of elements of the result matrix C = A x B, the number of elements is m x l
* Divide the elements of C by the number of processing units to assign the portion of elements to compute for each processor
* Master node sends to each worker information about: how many rows of matrix A they need, what row indices of matrix A to use, how many elements to compute, which column of matrix B to start from (i.e. column B offset).
* Each worker computes its elements and sends them back to the master node via MPI_Gatherv collective communication routine
* Master node gathers all received elements and prints them out on the file "outfile"

## Project structure

* source/
    * matrixMultiplicationS.c  -> *Sequential algorithm*
    * matrixMultiplicationP.c  -> *Parallel algorithm*
    * matrixOps.c -> *Matrix operations for creation, write and read*
    * matrixOps.h -> *Header file to include in source codes*
    * writeMatrix.c -> *Writes input matrices A and B into the file "test/infile"*
* test/ -> *Automatically generated folder at first writeMatrix execution*



## Getting Started

<ins>Remember to be in project root folder "MatrixMultiplication/" before proceeding</ins><br>
<ins>Create a bin folder at project root level where compiled files will be stored</ins><br>
Execute the following steps in order to have a fully running program:<br>

### Compilation

1. Compile the file *writeMatrix.c* with the following command:
```
gcc sourcewriteMatrix.c matrixOps.c -o bin/writeMatrix
```
2. Compile the file *matrixMultiplicationS.c* with the following command:
```
gcc source/matrixMultiplicationS.c source/matrixOps.c -o bin/seq
```
3. Compile the file *matrixMultiplicationP.c* with the following command:
```
gcc source/matrixMultiplicationP.c source/matrixOps.c -o bin/par
```

### Execution

<ins>Change directory to "MatrixMultiplication/bin" before proceeding with the following commands<ins>

1. Execute file *writeMatrix* with the following command:
```
./writeMatrix [rows A] [columns A] [rows B] [columns B]
```
Pass the four parameters to generate matrices A and B of the desired size

2. At this point you can:
    * Execute the sequential program with the following command:
    ```
    ./seq [rows A] [columns A] [rows B] [columns B]
    ```
    OR
    * Execute the parallel program on local machine with the following command:
    ```
    mpirun -np [number of cpus] par
    ```
    OR
    * Execute the parallel program on distributed environment with the following command:
    ```
    mpirun -np [number of cpus] --hostfile [hfile] par
    ```
<br>

# Solution evaluation

## Methodology
The tests were performed on an omogeneous AWS cluster composed by 8 [m4.xlarge](https://aws.amazon.com/ec2/instance-types/) instances.<br>
In order to evaluate the parallel algorithm, three parameters were considered: [speedup](#speedup), [strong scalability](#strong-scalability) and [weak scalability](#weak-scalability).<br>
All test inputs and timings can be found [here](benchmarks.txt).

### Speedup

The speedup is a measure that captures the relative benefit of solving a problem in parallel. It is defined as the ratio of the time taken to solve a problem on a single processing element to the time required to solve the same problem on a parallel computer/computing system with p identical processing elements.
Given an input I of size n, the time of the sequential solution on input I is denoted as T(1,n) and the time of the parallel solution on input I is denoted as T(p,n)
The relative speedup is **S(p,n)=T(1,n)T(p,n)**
<br>
Biggest experiment in terms of processing units:
<br>
![image](https://drive.google.com/uc?export=view&id=10sAXXnWtxCvfdJP2t6AvoYTdr-oVCNa4)

The results show how the best speedup rate is reached at 12 processors.
<br><br>

### Strong scalability

In this case the problem size stays fixed but the number of processing elements are increased. The goal is to minimize time to solution for a given problem.
<br>
 In strong scaling, a program is considered to scale linearly if the speedup (in terms of work units completed per unit time) is equal to the number of processing elements used ( N ).
<br>

1. Biggest experiment in terms of processing units:<br>
![image](https://drive.google.com/uc?export=view&id=1sJhACQfKuRMDPP4MNwzZvQ29LWSwDlI2)


2. Strong scaling efficiency: <br>
If the amount of time to complete a work unit with 1 processing element is t1, and the amount of time to complete the same unit of work with N processing elements is tN, the strong scaling efficiency (as a percentage of linear) is given as: **t1 / ( N * tN ) * 100%**

The following chart is the result of the average operation performed over 10 strong scalability tests with 10 different inputs<br>


![image](https://drive.google.com/uc?export=view&id=1sJhACQfKuRMDPP4MNwzZvQ29LWSwDlI2)
<br><br>

### Weak scalability

In this case the problem size (workload) assigned to each processing element stays constant and additional elements are used to solve a larger total problem.  In the case of weak scaling, linear scaling is achieved if the run time stays constant while the workload is increased in direct proportion to the number of processors.

1. Weak scaling:<br>
![image](https://drive.google.com/uc?export=view&id=1sJhACQfKuRMDPP4MNwzZvQ29LWSwDlI2)


2. Weak scaling efficiency: <br>
If the amount of time to complete a work unit with 1 processing element is t1, and the amount of time to complete N of the same work units with N processing elements is tN, the weak scaling efficiency (as a percentage of linear) is given as: **( t1 / tN ) * 100%**

![image](https://drive.google.com/uc?export=view&id=1sJhACQfKuRMDPP4MNwzZvQ29LWSwDlI2)

<br><br>


## Coding decisions

Here is the explanation of some coding design decisions made and the reasons why they were made:
* The input matrices values are in the range 0-255: the first choice was to allow matrices values to range from 0 to the max representable integer value INT_MAX. However, the multiplication and addition operations involved between matrices elements led to both integer overflow and long overflow when input matrices' sizes increased within the test cases.
* MPI_Ssend is used as Open MPI point-to-point communication routine: point-to-point communication works better in this parallel solution because workers can receive their workload immediately after it's been computed by the master node, otherwise, with collective communication routines, workers have to wait until the master node terminates to parse the input file and then receive their workload.


## Built With

* [Open MPI](https://www.open-mpi.org) - The C library used


## Author

* **Francesco Califano** - *Solution Design, implementation, testing and results presentation*  

## Credits

* *Measuring Parallel Scaling Performance* - [sharcnet](https://www.sharcnet.ca/help/index.php/Measuring_Parallel_Scaling_Performance)