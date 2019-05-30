The student must describe the solution and the benchmark in a README file, written in Markdown, and included in the submission. The README file must describe also the compilation phase and how to reproduce the results obtained.
# MatrixMultiplication
This repositories contains both the sequential and parallel version of the matrix multiplication algorithm. 
The algorithms are written in C language and the parallel version has been coded using the OpenMPI library.
The project has been developed with the aim of presenting the benefits of parallel programming by testing the algorithm with both strong and weak scalability.

The parallel solution was tested on an AWS cluster of 8 M4.xlarge instances 



## Getting Started

Clone the repository to have all the files needed for the execution.

## Strong scalability

Keeping the problem size fixed and pushing in more workers or processors. Goal: Minimize time to solution for a given problem.

If the amount of time to complete a work unit with 1 processing element is t1, and the amount of time to complete the same unit of work with N processing elements is tN, the strong scaling efficiency (as a percentage of linear) is given as: 
t1 / ( N * tN ) * 100%

## Weak scalability

Keeping the work per worker fixed and adding more workers/processors (the overall problem size increases). Goal: solve the larger problems.
To each processor is assigned the multiplication of two matrices with one thousand rows and one thousand columns.

### Prerequisites

Install OpenMPI with the following command: 

```
Give examples
```

### Installing

A step by step series of examples that tell you how to get a development env running

Say what the step will be

```
Give the example
```

And repeat

```
until finished
```

End with an example of getting some data out of the system or using it for a little demo


## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [OpenMPI](https://www.open-mpi.org) - The C library used


## Authors

* **Francesco Califano** - *Full Design and Implementation work* - [MatrixMultiplication](https://github.com/FranCali/MatrixMultiplication)

## Credits

* *Measuring Parallel Scaling Performance* - [sharcnet](https://www.sharcnet.ca/help/index.php/Measuring_Parallel_Scaling_Performance)
* **Carmine Spagnuolo** - *Ubuntu with OpenMPI and OpenMP* - [https://github.com/spagnuolocarmine/ubuntu-openmpi-openmp]