#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrixOps.h"

#define MAX_RAND 50
#define FILE_NAME "infile"

int main(int argc, char* argv[]){
    srand((unsigned) time(NULL));
    
    if(argc != 5){
        printf("Bad usage: ./a.out <rowsA> <colsA> <rowsB> <colsB>\n");
        exit(EXIT_FAILURE);
    }

    const int rowsA= atoi(argv[1]);//Number of rows matrix a
    const int colsA= atoi(argv[2]);//Number of columns matrix a
    const int rowsB= atoi(argv[3]);//Number of rows matrix b
    const int colsB= atoi(argv[4]);//Number of columns matrix b

    printf("Writing two matrices on file \"%s\"...\n", FILE_NAME);
    printf("Matrix A: %d rows %d columns\n", rowsA, colsA);
    printf("Matrix B: %d rows %d columns\n", rowsB, colsB);

    FILE* file = fopen(FILE_NAME, "w");

    if(file == NULL){
        perror("Error opening the file");
    }
    else{
        fprintf(file, "%d %d %d %d\n", rowsA, colsA, rowsB, colsB);
        writeRandMatrixOnFile(file, rowsA, colsA, MAX_RAND);
        fprintf(file, "\n");
        writeRandMatrixOnFile(file, rowsB, colsB, MAX_RAND);
    }

    fclose(file);
    return 0;
}

