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
gcc source/writeMatrix.c matrixOps.c -o bin/writeMatrix
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
In order to evaluate the parallel algorithm, three metrics were considered: [speedup](#speedup), [strong scalability](#strong-scalability) and [weak scalability](#weak-scalability).<br>
All test inputs and timings can be found [here](benchmarks.txt).

### Speedup

The speedup is a measure that captures the relative benefit of solving a problem in parallel. It is defined as the ratio of the time taken to solve a problem on a single processing element to the time required to solve the same problem on a parallel computer/computing system with p identical processing elements.
Given an input I of size n, the time of the sequential solution on input I is denoted as T(1,n) and the time of the parallel solution on input I is denoted as T(p,n)
The relative speedup is **S(p,n)=T(1,n)/T(p,n)**
<br>
Biggest experiment in terms of processing units and input size:
<br>

![image](https://drive.google.com/uc?export=view&id=1n4-OIQvCUhPmdbIdRAPYfk3J_2XxJnPH)

The results show how the best speedup rate is reached at 12 processors. The trend is an almost linear growing of the speedup, however it's clearly visible how the increasing number of processing elements affects the speedup because of the communication overhead between processors for work partitioning and partial results gathering. As the number of processors increases, it is further from the speedup upperbound which corresponds to the number of processors in that phase.
<br><br>

### Strong scalability

In this case the problem size stays fixed but the number of processing elements are increased. The goal is to minimize time to solution for a given problem.
<br>
 In strong scaling, a program is considered to scale linearly if the speedup (in terms of work units completed per unit time) is equal to the number of processing elements used ( N ).
<br>

1. Biggest experiment in terms of processing units:<br>
![image](https://drive.google.com/uc?export=view&id=1cmp5LqDG5DXuW23fZtVDqtrRRTC9YA7a)

This chart shows three strong scaling tests and their average values for each number of processing elements. The most significant improvement in terms of time to complete is with the biggest input size, this shows that the algorithm gains benefit with bigger inputs rather then inputs like the third test shown in the plot legend. In that case the time to complete has a slight decrease, not so significant to justify the adoption of more processing units though. Although is clearly visible the improve in time to complete with the increase of processing units, what the plot shows is that the more processors are used, the less the difference in time is remarkable. The plot shows how the function trend for all three tests and for the average values becomes almost the same from the use of 10 processing elements till the last tests with 16 elements. That said, the plot suggests, for the implemented solution, the use of at most 10 processing elements because the vantage after that is not remarkable.

2. Strong scaling efficiency: <br>
If the amount of time to complete a work unit with 1 processing element is t1, and the amount of time to complete the same unit of work with N processing elements is tN, the strong scaling efficiency (as a percentage of linear) is given as: **t1 / ( N * tN ) * 100%**

![image](https://drive.google.com/uc?export=view&id=1bsAtebFxPt5-fQUQ058N6CW3h1f9APRN)

This plot shows an interesting result. The exepcted trend should be a decreasing percentage of linear strong scaling efficiency because of the communication overhead for work partitioning and partial results gathering. However, the percentage of linear scaling efficiency is not always decreasing for every number of processing elements. They have points in which the efficiency is higher whith more processors than with less processors. These particular points show how in some cases the benefit gain from the addition of more workers is more relevant than the collaboration and communication overhead, resulting then in a higher scaling efficiency.
<br><br>

### Weak scalability

In this case the problem size (workload) assigned to each processing element stays constant and additional elements are used to solve a larger total problem.  In the case of weak scaling, linear scaling is achieved if the run time stays constant while the workload is increased in direct proportion to the number of processors.
For this test the workload assigned to a single processing element is A: 512x512, B: 512x512

Weak scaling efficiency: <br>
If the amount of time to complete a work unit with 1 processing element is t1, and the amount of time to complete N of the same work units with N processing elements is tN, the weak scaling efficiency (as a percentage of linear) is given as: **( t1 / tN ) * 100%**

![image](https://drive.google.com/uc?export=view&id=183GbtT0zwll_QKryHajML_HxAMD-oFJL)

The plot clearly shows that the percentage of linear scaling efficiency is low, the maximum value is reached with 2 processing units and after it quickly decreases under 10% going even further until the last point which has a percentage near to zero. With the inputs tested, this algorithm shows a poor weak scaling efficiency.
<br><br>


## Coding decisions

Here is the explanation of some coding design decisions made and the reasons why they were made:
* The input matrices values are in the range 0-255: the first choice was to allow matrices values to range from 0 to the max representable integer value INT_MAX. However, the multiplication and addition operations involved between matrices elements led to both integer overflow and long overflow when input matrices' sizes increased within the test cases.
* MPI_Ssend is used as Open MPI point-to-point communication routine: point-to-point communication works better in this parallel solution because workers can receive their workload immediately after it's been computed by the master node, otherwise, with collective communication routines, workers have to wait until the master node terminates to parse the input file and then receive their workload. Furthermore, the need of using the synchronous send is because other types of send routines cause some buffer-related issues when increasing the input. 


## Built With

* [Open MPI](https://www.open-mpi.org) - The C library used


## Author

* **Francesco Califano** - *Solution Design, implementation, testing and results presentation*  

## Credits

* *Measuring Parallel Scaling Performance* - [sharcnet](https://www.sharcnet.ca/help/index.php/Measuring_Parallel_Scaling_Performance)