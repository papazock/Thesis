#include "globals.h"

//Version 011
//Verified at 2014/06/01



void motion_compensation2(uint8 *previous, int *vectors_x, int *vectors_y, uint8 *output) {
    int x, y, k, l;

    for (x = 0; x < N_B; x++)       /* For all blocks in the current frame */
        for (y = 0; y < M_B; y++) {
            for (k = 0; k < B; k++)       /* For all pixels in the block */
                for (l = 0; l < B; l++)
                    if ((B * x + vectors_x[x * M_B + y] + k) < 0) {
                        //output[B*x+k][B*y+l]=previous[B*x][B*y+l];
                        output[(B * x + k) * M + B * y + l] = previous[(B * x) * M + B * y + l];
                        //printf("1");
                    } else if (B * y + vectors_y[x * M_B + y] + l < 0) {
                        //output[B*x+k][B*y+l]=previous[B*x+k][B*y];
                        output[(B * x + k) * M + B * y + l] = previous[(B * x + k) * M + B * y];
                        //printf("2");
                    } else if ((B * x + vectors_x[x * M_B + y] + k) > (N - 1)) {
                        //output[B*x+k][B*y+l]=previous[B*x+B-1][B*y+l];
                        output[(B * x + k) * M + B * y + l] = previous[(B * x + B - 1) * M + B * y + l];
                        //printf("3");
                    } else if ((B * y + vectors_y[x * M_B + y] + l) > (M - 1)) {
                        //output[B*x+k][B*y+l]=previous[B*x+k][B*y+B-1];
                        output[(B * x + k) * M + B * y + l] = previous[(B * x + k) * M + B * y + B - 1];
                        //printf("4");
                    } else {
                        //output[B*x+k][B*y+l]=previous[B*x+vectors_x[x][y]+k][B*y+vectors_y[x][y]+l];
                        //printf(".");
                        output[(B * x + k) * M + B * y + l] = previous[(B * x + vectors_x[x * M_B + y] + k) * M +
                                                                       B * y + vectors_y[x * M_B + y] + l];
                    }
        }

}


void motion_compensationFRAME(uint8 previous[N][M], int vectors_x[N / B][M / B], int vectors_y[N / B][M / B],
                              uint8 output[N][M]) {
    int x, y, k, l;

    for (x = 0; x < N / B; x++)       /* For all blocks in the current frame */
        for (y = 0; y < M / B; y++) {
            for (k = 0; k < B; k++)       /* For all pixels in the block */
                for (l = 0; l < B; l++)
                    if ((B * x + vectors_x[x][y] + k) < 0)
                        output[B * x + k][B * y + l] = previous[B * x][B * y + l];
                    else if (B * y + vectors_y[x][y] + l < 0)
                        output[B * x + k][B * y + l] = previous[B * x + k][B * y];
                    else if ((B * x + vectors_x[x][y] + k) > (N - 1))
                        output[B * x + k][B * y + l] = previous[B * x + B - 1][B * y + l];
                    else if ((B * y + vectors_y[x][y] + l) > (M - 1))
                        output[B * x + k][B * y + l] = previous[B * x + k][B * y + B - 1];
                    else
                        output[B * x + k][B * y + l] = previous[B * x + vectors_x[x][y] + k][B * y + vectors_y[x][y] +
                                                                                             l];
        }

}
 
