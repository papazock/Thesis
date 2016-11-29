//
// Created by Chris Papazoglou on 24/11/2016.
//

#ifndef THESIS_HELPER_H
#define THESIS_HELPER_H
typedef struct TheResult {
    int **motionVect;
    int computations;
    int mvRow;
    int mvCol;
} TheResult;


int costFuncMAD(int **currentBlk, int **refBlk, int n);

int getCol(int **imgI);

int getRow(int **imgI);

void initSDSP(int pInt[5][2]);

void setValuesAt(int **ldsp, int i, int i1, int i2);

int **createVector(int row, int col, int val);

int **getSubArray(int **p, int row_start, int row_end, int col_start, int col_end);

void min(int **pInt, int *minVal, int *minPoint, int n);

TheResult *createReturnStruct(int **motionVectors, int computations, int mRow, int mCol);

void minCost(int **costs, int *dx, int *dy, int *min, int _size);

int **createVectorTest(int row, int col);

#endif //THESIS_HELPER_H
