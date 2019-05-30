#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
#include "matrixOps.h"

#define FILE_NAME "infile"
#define MASTER 0
#define SIZE (sizeof(int) * rowsA) + (2 * sizeof(int))

int main(int argc, char* argv[]) {

    double startTime = MPI_Wtime();
    srand((unsigned) time(NULL));

    int tasksNum;                      /*Number of tasks in the communicator*/
    int taskId;                        /*Task's rank in the communicator*/
    int rowsA, colsA, rowsB, colsB;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &tasksNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskId);
    MPI_Status status;

    FILE *inFile = fopen(FILE_NAME, "r");

    if(inFile == NULL){
        printf("Error opening the file\n");
        exit(EXIT_FAILURE);
    }

    fscanf(inFile, " %d %d %d %d", &rowsA, &colsA, &rowsB, &colsB);           /*Reading rows and columns of matrix A and matrix B from the first row of the file "infile"*/
   
    if(colsA != rowsB){
        printf("Cannot compute A X B -> matrices have to be of the form A[m][n] B[n][l], m can be equal to l\n");
        exit(EXIT_FAILURE);
    }else if(tasksNum > rowsA * colsB){
        printf("The problem size is too small for the execution in parallel with %d tasks\n", tasksNum);
        exit(EXIT_FAILURE);
    }

    const int portion = rowsA * colsB / tasksNum; 
    const int reminder = (rowsA * colsB) % tasksNum;

    int *recvBuf = malloc(SIZE);      /*Buffer to store: <row indexes of matrix A assigned to the task><offset from column 0 of matrix B><number of elements of product matrix to compute>*/  
    int rows;                         /*Number of rows of matrix A to compute the assigned elements*/

    if(taskId == MASTER){
        
        int elements = 0;                   /*Current elements collected*/
        int taskElements;                   /*Number of elements assigned to the task*/
        int taskRank = 0;                   /*Current task rank*/     
        int colBOffset = 0;                 /*Offset from column 0 of matrix B*/
        int rowsCount = 0; 
        int *indexbuf = malloc(SIZE);
        int index = 0;

        taskElements = (taskRank < reminder) ? portion + 1 : portion;
            
        for(int row = 0; row < rowsA; row++){
            rowsCount++;
            indexbuf[index++] = row;

            for(int col = 0; col < colsB; col++){
                elements++;
                
                if(elements == taskElements){

                    (taskRank == 0) ? rows = rowsCount : MPI_Ssend(&rowsCount, 1, MPI_INT, taskRank, 0, MPI_COMM_WORLD);
                    
                    indexbuf[rowsCount] = colBOffset; indexbuf[rowsCount + 1] = elements;
                    
                    if(taskRank == 0){
                       for(int i = 0; i < rowsCount + 2; i++)
                            recvBuf[i] = indexbuf[i];
                    }else{
                        MPI_Ssend(indexbuf, rowsCount + 2, MPI_INT, taskRank, 0, MPI_COMM_WORLD);
                    }

                    colBOffset = (colBOffset  + elements) % colsB;
                    index = 0;

                    if(col < colsB - 1){        /*Save row index for next task before moving on with row++*/
                        indexbuf[index++] = row;
                        rowsCount = 1;
                    }  
                    else
                        rowsCount = 0;
                    
                    elements = 0;
                    taskRank++;

                    taskElements = (taskRank < reminder) ? portion + 1 : portion;
                    
                }
            }
            
        }
        free(indexbuf);
    }else{
        MPI_Recv(&rows, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(recvBuf, rows + 2, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &status);   
    }
    
    int offset = recvBuf[rows], elements = recvBuf[rows + 1];
    int **partMatrixA = allocateMatrix(rows, colsA);
    int **matrixB = allocateMatrix(rowsB, colsB);
    
    for(int i = 0, j = 0, next; i < rowsA; i++){            /*Reading selected rows from matrix A*/
        if(i == recvBuf[j] && j < rows){
            for(int k = 0; k < colsA; k++)
                fscanf(inFile, " %d", &partMatrixA[j][k]);
            j++;
        }else{
            for(int k = 0; k < colsA; k++)
                fscanf(inFile, " %d", &next);
        }
    }
    
    for(int i = 0; i < rowsB; i++){                         /*Reading matrix B from file*/
        for(int j = 0; j < colsB; j++)
            fscanf(inFile, " %d", &matrixB[i][j]);   
    }

    long int *sendBuf = multiplyMatricesretVect(partMatrixA, matrixB, rows, colsA, rowsB, colsB, offset, elements);   /*Buffer used to store computed values to send to MASTER*/ 
    long int *recvGatBuf = malloc(sizeof(long int) * elements * tasksNum);
    int recvCounts[tasksNum];
    int displs[tasksNum];
    displs[0] = 0;

    for(int taskRank = 0; taskRank < tasksNum; taskRank++){
        recvCounts[taskRank] = (taskRank < reminder) ? portion + 1 : portion;
        displs[taskRank+1] = displs[taskRank] + recvCounts[taskRank]; 
    }

    MPI_Gatherv(sendBuf, elements, MPI_LONG, recvGatBuf, recvCounts, displs, MPI_LONG, MASTER, MPI_COMM_WORLD);

    if(taskId == MASTER){
        FILE *outFile = fopen("outfile", "w+");
        fprintf(outFile, "%d %d\n", rowsA, colsB);
        for(int i = 0; i < rowsA * colsB; i++){
            fprintf(outFile, "%ld ", recvGatBuf[i]);
            if((i + 1) % colsB == 0)
                fprintf(outFile, "\n");
        }
        fclose(outFile);
    }
    
    fclose(inFile);
    free(recvBuf);
    free(recvGatBuf);
    free(sendBuf);
    free(matrixB);
    free(partMatrixA);
    
    MPI_Finalize();
    double endTime = MPI_Wtime();

    if(taskId == MASTER){
        printf("The computation took %.2f seconds\nOpen the file \"outfile\" to see the result matrix\n", endTime - startTime);
    }
    return 0;
}