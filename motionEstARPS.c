//
// Created by Chris Papazoglou on 15/11/2016.
//


#include <stdlib.h>
#include <stdio.h>
#include "motionEstARPS.h"

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

TheResult *motionEstARPS(int **imgP, int **imgI, int mbSize, int p, int _row, int _col) {

    int row = _row;
    int col = _col;
    int **SDSP = createVector(5, 2, 0);
    int **LDSP = createVector(6, 2, 0);;
    int **vectors = createVector(2, row * col / mbSize ^ 2, 0);
    int **costs = createVector(1, 6, 65537);
    initSDSP(SDSP);
    int cost;
    int point;
    int costsSize = 6;


    // We will be storing the positions of points where the checking has been
    // already done in a matrix that is initialised to zero. As one point is
    // checked, we set the corresponding element in the matrix to one.
    int **checkMatrix = createVector(2 * p + 1, 2 * p + 1, 0);
    int computations = 0;

    // we start off from the top left of the image
    //we will walk in steps of mbSize
    // mbCount will keep track of how many blocks we have evaluated
    int mbCount = 0;
    int i, j;
    int stepSize = 0;
    int maxIndex = 0;
    int x, y;
    int **subA;
    int **subB;

    for (i = 0; i < row; i += mbSize) {

        for (j = 0; j < col; j += mbSize) {




            //the Adapive Rood Pattern search starts
            // we are scanning in raster order

            x = j;
            y = i;

            subA = getSubArray(imgP, i, i + mbSize - 1, j, j + mbSize - 1);
            subB = getSubArray(imgI, i, i + mbSize - 1, j, j + mbSize - 1);
            costs[0][2] = costFuncMAD(subA, subB, mbSize);
            free(subA);
            free(subB);

            checkMatrix[p + 1][p + 1] = 1;

            computations++;
            // if we are in the left most column then we have to make sure that
            //  we just do the LDSP with stepSize = 2

            if (j - 1 < 0) {
                stepSize = 2;
                maxIndex = 5;
            } else {
                stepSize = max(abs(vectors[0][mbCount - 1]), abs(vectors[1][mbCount - 1]));

                //now we have to make sure that if the point due to motion
                // vector is one of the LDSP points then we dont calculate it
                // again
                if ((abs(vectors[0][mbCount - 1]) == stepSize && vectors[1][mbCount - 1] == 0)
                    || (abs(vectors[1][mbCount - 1]) == stepSize && vectors[0][mbCount - 1] == 0)) {

                    maxIndex = 5; // we just have to check at the rood pattern 5 points
                } else {
                    maxIndex = 6; // we have to check 6 points
                    setValuesAt(LDSP, 5, vectors[1][mbCount - 1], vectors[0][mbCount - 1]);
                }
            }


            //The index points for first and only Large Diamond search pattern

            setValuesAt(LDSP, 0, 0, -stepSize);
            setValuesAt(LDSP, 1, -stepSize, 0);
            setValuesAt(LDSP, 2, 0, 0);
            setValuesAt(LDSP, 3, stepSize, 0);
            setValuesAt(LDSP, 4, 0, stepSize);


            int k;
            for (k = 0; k < maxIndex; k++) {
                int refBlkVer = y + LDSP[k][1];   // row/Vert co-ordinate for ref block
                int refBlkHor = x + LDSP[k][0];   // col/Horizontal co-ordinate

                if (refBlkVer < 0 || refBlkVer + mbSize - 1 >= row || refBlkHor < 0 || refBlkHor + mbSize - 1 >= col) {

                    continue; // outside image boundary
                }

                if (k == 2 || stepSize == 0) {
                    continue; //center point already calculated
                }

                subA = getSubArray(imgP, i, i + mbSize - 1, j, j + mbSize - 1);
                subB = getSubArray(imgI, refBlkVer, refBlkVer + mbSize - 1, refBlkHor, refBlkHor + mbSize - 1);

                costs[0][k] = costFuncMAD(subA, subB, mbSize);

                free(subA);
                free(subB);

                computations++;
                checkMatrix[LDSP[k][1] + p][LDSP[k][0] + p] = 1;
            }


            min(costs, &cost, &point, costsSize);



            // The doneFlag is set to 1 when the minimum
            // is at the center of the diamond

            x = x + LDSP[point][0];
            y = y + LDSP[point][1];
            free(costs);
            costs = createVector(1, 5, 65537);
            costsSize = 5;
            costs[0][2] = cost;


            int doneFlag = 0;
            while (doneFlag == 0) {
                for (k = 0; k < 5; k++) {
                    int refBlkVer = y + SDSP[k][1];   // row/Vert co-ordinate for ref block
                    int refBlkHor = x + SDSP[k][0];   // col/Horizontal co-ordinate
                    if (refBlkVer < 0 || refBlkVer + mbSize - 1 >= row || refBlkHor < 0 ||
                        refBlkHor + mbSize - 1 >= col) {
                        continue;
                    }


                    //printf("x= %d ,  y= %d\n",y - i + SDSP[k][1] + p + 1,x - j + SDSP[k][0] + p + 1);
                    fflush(NULL);
                    if (k == 2) {
                        continue;
                    } else if (refBlkHor < j - p || refBlkHor > j + p || refBlkVer < i - p || refBlkVer > i + p) {
                        continue;
                    } else if (checkMatrix[y - i + SDSP[k][1] + p][x - j + SDSP[k][0] + p] == 1) {
                        continue;
                    }

                    subA = getSubArray(imgP, i, i + mbSize - 1, j, j + mbSize - 1);
                    subB = getSubArray(imgI, refBlkVer, refBlkVer + mbSize - 1, refBlkHor, refBlkHor + mbSize - 1);

                    costs[0][k] = costFuncMAD(subA, subB, mbSize);

                    free(subA);
                    free(subB);

                    checkMatrix[y - i + SDSP[k][1] + p][x - j + SDSP[k][0] + p] = 1;
                    computations++;


                }

                min(costs, &cost, &point, costsSize);

                if (point == 2) {
                    doneFlag = 1;
                } else {
                    x = x + SDSP[point][0];
                    y = y + SDSP[point][1];
                    free(costs);
                    costs = createVector(1, 5, 65537);
                    costsSize = 5;
                    costs[0][2] = cost;
                }

            }// while loop ends here

            vectors[0][mbCount] = y - i;  // row co-ordinate for the vector
            vectors[1][mbCount] = x - j;  // col co-ordinate for the vector
            mbCount = mbCount + 1;
            free(costs);
            costs = createVector(1, 6, 65537);
            costsSize = 6;

            free(checkMatrix);
            checkMatrix = createVector(2 * p + 1, 2 * p + 1, 0);
        }
    }

    return createReturnStruct(vectors, computations / (mbCount - 1), 2, row * col / mbSize ^ 2);

}




