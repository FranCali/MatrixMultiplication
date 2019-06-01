# MatrixMultiplication
The project contains both the sequential and parallel version of the matrix multiplication algorithm.<br>
The aim of this work is to compare both the parallel and the sequential algorithms for matrix multiplication and explain, using parallel computing parameters, the difference in performance and the benefits (if any) of the parallel solution.<br>
The algorithms are both written in C programming language. 
The parallel algorithm has been coded using the OpenMPI library.

### Project structure
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
<ins>Change directory to bin folder before proceeding with the following command<ins>
```
cd bin/
```
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


## Methodology
The tests were performed on an omogeneous AWS cluster composed by 8 [m4.xlarge](https://aws.amazon.com/ec2/instance-types/) instances.<br>
In order to test the performance of the parallel algorithm, two parameters were considered: [strong scalability](#strong-scalability) and [weak scalability](#weak-scalability).<br>
Furthermore, the speedup of the parallel solution was computed in order to relate the parallel and the sequential solutions.
All test inputs and results are shown in [testCases](testCases.txt) file.

## Strong scalability
In this case the problem size stays fixed but the number of processing elements are increased.<br>
Goal: Minimize time to solution for a given problem.<br>
In strong scaling, a program is considered to scale linearly if the speedup(in terms of work units completed per unit time) is equal to the number ofprocessing elements used ( N ). In general, it is harder to achieve goodstrong-scaling at larger process counts since the communication overheadfor many/most algorithms increases in proportion to the number of processesused.<br>
For the strong scalability, 10 tests were performed and than the final result as shown in figure 1 is the average value for each number of cpus, from 1 (sequential algorithm) to 16 (i.e. number of cores per instance times number of AWS instances).<br>

If the amount of time to complete a work unit with 1 processing element is t1, and the amount of time to complete the same unit of work with N processing elements is tN, the strong scaling efficiency (as a percentage of linear) is given as: 
**t1 / ( N * tN ) * 100%**

## Weak scalability

In this case the problem size (workload) assigned to each processing element stays constant and additional elements are used to solve a larger total problem (one that wouldn't fit in RAM on a single node, for example). Therefore, this type of measurement is justification for programs that take a lot of memory or other system resources (something that is memory-bound). In the case of weak scaling, linear scaling is achieved if the run time stays constant while the workload is increased in direct proportion to the number of processors. Most programs running in this mode should scale well to larger core counts as they typically employ nearest-neighbour communication patterns where the communication overhead is relatively constant regardless of the number of processes used; exceptions include algorithms that employ heavy use of global communication patterns, eg. FFTs and transposes. 


## Built With

* [OpenMPI](https://www.open-mpi.org) - The C library used


## Author

* **Francesco Califano** - *Solution Design, implementation, testing and results presentation*  

## Credits

* *Measuring Parallel Scaling Performance* - [sharcnet](https://www.sharcnet.ca/help/index.php/Measuring_Parallel_Scaling_Performance)