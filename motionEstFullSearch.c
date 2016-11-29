//
// Created by Chris Papazoglou on 28/11/2016.
//

#include <stdlib.h>
#include "motionEstFullSearch.h"


TheResult *motionEstES(int **imgP, int **imgI, int mbSize, int p, int _row, int _col) {

    int row = _row;
    int col = _col;

    int **vectors = createVector(2, row * col / mbSize ^ 2, 0);
    int **costs = createVector(2 * p + 1, 2 * p + 1, 65537);

    int **subA;
    int **subB;

    int computations = 0;


// we start off from the top left of the image
// we will walk in steps of mbSize
// for every marcoblock that we look at we will look for
// a close match p pixels on the left, right, top and bottom of it
    int mbCount = 0;
    int i, j;
    for (i = 0; i < row; i += mbSize) {
        for (j = 0; j < col; j += mbSize) {

            // the exhaustive search starts here
            //we will evaluate cost for  (2p + 1) blocks vertically
            // and (2p + 1) blocks horizontaly
            // m is row(vertical) index
            // this means we are scanning in raster order

            int m, n;
            for (m = -p; m <= p; m++) {
                for (n = -p; n <= p; n++) {
                    int refBlkVer = i + m;   // row/Vert co-ordinate for ref block
                    int refBlkHor = j + n;   // col/Horizontal co-ordinate

                    if (refBlkVer < 0 || refBlkVer + mbSize >= row || refBlkHor < 0 || refBlkHor + mbSize >= col)
                        continue;

                    subA = getSubArray(imgP, i, i + mbSize - 1, j, j + mbSize - 1);
                    subB = getSubArray(imgI, refBlkVer, refBlkVer + mbSize - 1, refBlkHor, refBlkHor + mbSize - 1);

                    costs[m + p][n + p] = costFuncMAD(subA, subB, mbSize);
                    computations++;
                    free(subA);
                    free(subB);

                }
            }


            // Now we find the vector where the cost is minimum
            // and store it ... this is what will be passed back.

            int dx, dy, _min;

            minCost(costs, &dx, &dy, &_min, 2 * p + 1); // finds which macroblock in imgI gave us min Cost
            vectors[0][mbCount] = dy - p - 1;    // row co-ordinate for the vector
            vectors[1][mbCount] = dx - p - 1;    // col co-ordinate for the vector
            mbCount = mbCount + 1;
            costs = createVector(2 * p + 1, 2 * p + 1, 65537);

        }
    }

    return createReturnStruct(vectors, computations / (mbCount - 1), 2, row * col / mbSize ^ 2);
}

