//Sequential version of matrix multiplication algorithm
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "matrixOps.h"

#define FILE_NAME "infile"
#define MAX_RAND 5


int main(int argc, char* argv[]){

    time_t startTime,endTime;
    time(&startTime);
    srand((unsigned) time(NULL));
    FILE* inFile = fopen(FILE_NAME, "r");

    if(inFile == NULL){
        printf("Error opening the file \"%s\"\n", FILE_NAME);
        exit(EXIT_FAILURE);
    }

    int rowsA, colsA, rowsB, colsB;    
    
    fscanf(inFile, "%d %d %d %d", &rowsA, &colsA, &rowsB, &colsB);
    int **matrixA = readMatrixFromFile(inFile, rowsA, colsA);
    int **matrixB = readMatrixFromFile(inFile, rowsB, colsB);
    fclose(inFile);

    int **prodMatrix = multiplyMatrices(matrixA, matrixB, rowsA, colsA, rowsB, colsB);

    if(prodMatrix==NULL)    /*Product was not appliable on that matrices*/
        exit(EXIT_FAILURE);
    
    FILE *outFile = fopen("outfile", "w");
    writeMatrixOnFile(outFile, prodMatrix, rowsA, colsB);
    fclose(outFile);

    free(prodMatrix);
    free(matrixA);
    free(matrixB);
    
    time(&endTime);
    printf("The computation took %ld seconds\nOpen file \"outfile\" to see the result matrix\n", endTime-startTime);
    return 0;
}
