#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "matrix_ops.h"

#define FILE_NAME "infile.txt"
#define MAX_RAND 10
#define MASTER 0

int main(int argc, char* argv[]) {

    double starttime = MPI_Wtime();
    srand((unsigned) time(NULL));
    int num_tasks;
    int task_id;
    const int rows_a=3, cols_a=3, rows_b=3, cols_b=2;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &task_id);

    int **matrix_a = instantiateRandMatrix(rows_a, cols_a, MAX_RAND);
    int **matrix_b = instantiateRandMatrix(rows_b, cols_b, MAX_RAND);
    int recv_index;//Here I'll store the index of the matrix a's row
    int *rows_a_index = malloc (sizeof(int) * rows_a);//Buffer with values from 0 to rows_a-1
    int *recv_rows_buf = malloc(rows_a * cols_b * sizeof(int));//Buffer where MASTER stores partial rows results
    int rows_per_task = rows_a / num_tasks;
    int reminder = rows_a % num_tasks;
    int sv_recv_index[rows_per_task + 1];
    
    for(int i = 0; i < rows_a; i++)
        rows_a_index[i] = i;

    int sendcounts[num_tasks];
    int displs[num_tasks];
    displs[0] = 0;

    for(int i = 0; i < num_tasks; i++){
        if(i < reminder)
            sendcounts[i] = rows_per_task + 1;
        else
            sendcounts[i] = rows_per_task;
    
        displs[i+1] = displs[i] + sendcounts[i];
    }


    if(task_id == MASTER){
        printMatrix(matrix_a, rows_a, cols_a);
        printMatrix(matrix_b, rows_b, cols_b);
        printf("\n");
    }
    MPI_Scatterv(rows_a_index, sendcounts, displs, MPI_INT, sv_recv_index, rows_per_task+1, MPI_INT, MASTER,MPI_COMM_WORLD);

    
    int size;
    //printf("I'm task %d \n", task_id);
    if(task_id < reminder)
        size = rows_per_task+1;
    else
        size = rows_per_task;
    
    //printf("Sono il task %d e devo calcolare %d righe\n", task_id, size);

    //Compute the single product matrix row
    int* partial_matrix = malloc(sizeof(int) * size * cols_b);
    int* partial_vector = malloc(sizeof(int) * cols_b);

    for(int i = 0; i < size; i++){
        partial_vector = multiplyVectMatrix(*(matrix_a + sv_recv_index[i]), matrix_b, cols_a, rows_b, cols_b);
        for(int i =0; i < cols_b; i++){
            partial_matrix[i] = partial_vector[i];
        }
        partial_matrix+=cols_b;
    }
    
    partial_matrix -= (cols_b * size);
    
    int recvcounts[num_tasks];
    int recvdispls[num_tasks];
    recvdispls[0] = 0;
    for(int i = 0; i < num_tasks; i++){
        
        recvcounts[i] = cols_b * size;
        recvdispls[i+1] = recvdispls[i] + recvcounts[i];
    }

    MPI_Gatherv(partial_matrix, cols_b * size, MPI_INT, recv_rows_buf, recvcounts, recvdispls, MPI_INT, MASTER, MPI_COMM_WORLD);

    if(task_id == MASTER){
        for(int i = 0; i < cols_b * rows_a; i++){
            printf("%d |", recv_rows_buf[i]);
            if((i % cols_b-1) == 0)
                printf("\n");
        }
    }

    free(partial_vector);
    free(partial_matrix);
    free(recv_rows_buf);
    free(rows_a_index);
    free(matrix_a);
    free(matrix_b);
    MPI_Finalize();
    double endtime = MPI_Wtime();
    if(task_id == MASTER)
        printf("The computation took %f seconds\n", endtime - starttime);
    return 0;
}



