# MatrixMultiplication
The project contains both the sequential and parallel version of the matrix multiplication algorithm.<br>
The aim of this work is to compare both the parallel and the sequential algorithms for matrix multiplication and explain, using parallel computing parameters, the difference in performance and the benefits (if any) of the parallel solution.<br>
The algorithms are both written in C programming language. 
The parallel algorithm has been coded using the OpenMPI library.

## Methodology
The test were performed on an omogeneous AWS cluster composed by 8 [m4.xlarge](https://aws.amazon.com/ec2/instance-types/) instances.<br>
In order to test the performance of the parallel algorithm, two parameters were considered: [strong scalability](#strong-scalability) and [weak scalability](#weak-scalability).<br>
Furthermore, the speedup of the parallel solution was computed in order to relate the parallel and the sequential solutions.

### Project structure
* source/
    * matrixMultiplicationS.c  -> *Sequential algorithm*
    * matrixMultiplicationP.c  -> *Parallel algorithm*
    * matrixOps.c -> *Matrix operations for creation, write and read*
    * matrixOps.h -> *Header file to include in source codes*
    * writeMatrix.c -> *Input file generator*


## Getting Started


```
Give examples
```

## Strong scalability

Keeping the problem size fixed and pushing in more workers or processors. Goal: Minimize time to solution for a given problem.<br>
For the strong scalability, 10 tests were executed and than the final result as shown in figure 1 is the average value for each cpu number, from 1 (sequential algorithm) to 18 (that is the number of cpus times number of cores per AWS instance).<br>

If the amount of time to complete a work unit with 1 processing element is t1, and the amount of time to complete the same unit of work with N processing elements is tN, the strong scaling efficiency (as a percentage of linear) is given as: 
t1 / ( N * tN ) * 100%

## Weak scalability

Keeping the work per worker fixed and adding more workers/processors (the overall problem size increases). Goal: solve the larger problems.
To each processor is assigned the multiplication of two matrices with one thousand rows and one thousand columns.


## Built With

* [OpenMPI](https://www.open-mpi.org) - The C library used


## Author

* **Francesco Califano** - *Solution Design, implementation, testing and results presentation*  

## Credits

* *Measuring Parallel Scaling Performance* - [sharcnet](https://www.sharcnet.ca/help/index.php/Measuring_Parallel_Scaling_Performance)