//
// Created by Chris Papazoglou on 24/11/2016.
//

#include "Helper.h"
#include <stdlib.h>

TheResult *createReturnStruct(int **motionVectors, int computations, int mRow, int mCol) {
    TheResult *result = malloc(sizeof(TheResult));
    result->motionVect = motionVectors;
    result->computations = computations;
    result->mvRow = mRow;
    result->mvCol = mCol;
    return result;
}


void min(int **pInt, int *minVal, int *minPoint, int n) {
    int c, min, index;

    min = pInt[0][0];
    index = 0;
    for (c = 1; c < n; c++) {
        if (pInt[0][c] < min) {
            index = c;
            pInt[0][c];
        }
    }

    *minVal = pInt[0][index];
    *minPoint = index;
}


int **getSubArray(int **p, int row_start, int row_end, int col_start, int col_end) {
    int i, j;
    int total_rows = row_end - row_start + 1;
    int total_cols = col_end - col_start + 1;
    int **arr = malloc(sizeof(int *) * total_rows);

    for (i = 0; i < total_rows; i++) {
        arr[i] = malloc(sizeof(int) * total_cols);
        for (j = 0; j < total_cols; j++) {
            arr[i][j] = p[i + row_start][j + col_start];
        }
    }

    return arr;


}

void setValuesAt(int **ldsp, int at, int val1, int val2) {
    ldsp[at][0] = val1;
    ldsp[at][1] = val2;
}

void initSDSP(int pInt[5][2]) {
//    SDSP(1,:) = [ 0 -1];
    setValuesAt(pInt, 0, 0, -1);
//    SDSP(2,:) = [-1  0];
    setValuesAt(pInt, 1, -1, 0);
//    SDSP(3,:) = [ 0  0];
    setValuesAt(pInt, 2, 0, 0);
//    SDSP(4,:) = [ 1  0];
    setValuesAt(pInt, 3, 1, 0);
//    SDSP(5,:) = [ 0  1];
    setValuesAt(pInt, 4, 0, 1);

}

int **createVector(int row, int col, int val) {

    int i, j, **intMatrix;

    //create an array int* with #row rows
    intMatrix = (int **) malloc(row * sizeof(int *));

    //for each row create #col columns
    for (i = 0; i < row; ++i) {
        intMatrix[i] = (int *) malloc(col * sizeof(int));
        //init each cell of row i with value val
        for (j = 0; j < col; j++) {
            intMatrix[i][j] = val;
        }
    }

    return intMatrix;


}

int costFuncMAD(int **currentBlk, int **refBlk, int n) {
    int err = 0;
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            err = err + abs((currentBlk[i][j] - refBlk[i][j]));
        }
    }


    return err / (n * n);

}

void minCost(int **costs, int *dx, int *dy, int *min, int _size) {

    int row = _size;
    int col = _size;

//
//% we check whether the current
//% value of costs is less then the already present value in min. If its
//% inded smaller then we swap the min value with the current one and note
//% the indices.

    *min = 65537;
    int i, j;
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            if (costs[i][j] < (*min)) {
                *min = costs[i][j];
                *dx = j;
                *dy = i;
            }
        }
    }
}

int **createVectorTest(int row, int col) {

    int i, j, **intMatrix;

    //create an array int* with #row rows
    intMatrix = (int **) malloc(row * sizeof(int *));

    //for each row create #col columns
    for (i = 0; i < row; ++i) {
        intMatrix[i] = (int *) malloc(col * sizeof(int));
        //init each cell of row i with value val
        for (j = 0; j < col; j++) {
            intMatrix[i][j] = rand() % 256;
        }
    }

    return intMatrix;


}



