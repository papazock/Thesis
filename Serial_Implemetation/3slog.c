/* 3 step logarithmic search with motion compensation and SNR computation
   Minas Dasygenis 2013
   Compile it with gcc 3slog.c  -lm */

/* Update 2014 with parametrized input */
#include <stdlib.h>
#include "globals.h"


void log_motion_estimation(uint8 *current, uint8 *previous, int *vectors_x, int *vectors_y) {
    int x, y, i, j, k, l, p1, p2, dist, dd, min, c, b, bestx, besty;
    for (c = 0; c < N_B; c++)
        for (b = 0; b < M_B; b++) {
            vectors_x[c * M_B + b] = 0;
            vectors_y[c * M_B + b] = 0;
        }

    for (x = 0; x < N_B; x++)            /* For all blocks in the current frame */
        for (y = 0; y < M_B; y++) {
            //d=4; ->> 4 2 1 values
            //while(d>0)
            for (dd = 4; dd > 1; dd--)  //--> gives 4 3 2
                if (dd == 4) {
                    //d=4;
                    min = 255 * B * B;

                    for (i = -4; i < 4 + 1; i += 4)      /* For all candidate blocks */
                        for (j = -4; j < 4 + 1; j += 4) {
                            dist = 0;
                            for (k = 0; k < B; k++)        /* For all pixels in the block */
                                for (l = 0; l < B; l++) {
                                    int tmp9 = vectors_x[x * M_B + y];
                                    p1 = current[(B * x + k) * M + B * y + l];
                                    if ((B * x + tmp9 + i + k) < 0 || (B * x + tmp9 + i + k) > (N - 1) ||
                                        (B * y + tmp9 + j + l) < 0 || (B * y + tmp9 + j + l) > (M - 1)) {
                                        p2 = 0;
                                    } else {
                                        p2 = previous[(B * x + tmp9 + i + k) * M + B * y + tmp9 + j + l];
                                    }


                                    dist += abs(p1 - p2);

                                }
                            if (dist < min) {
                                min = dist;
                                bestx = i;
                                besty = j;
                            }
                        }
                    // d=d/2;

                    vectors_x[x * M_B + y] += bestx;
                    vectors_y[x * M_B + y] += besty;
                } else if (dd == 3) {
                    //d=2;
                    min = 255 * B * B;

                    for (i = -2; i < 2 + 1; i += 2)      /* For all candidate blocks */
                        for (j = -2; j < 2 + 1; j += 2) {
                            dist = 0;
                            for (k = 0; k < B; k++)        /* For all pixels in the block */
                                for (l = 0; l < B; l++) {
                                    int tmp9 = vectors_x[x * M_B + y];
                                    p1 = current[(B * x + k) * M + B * y + l];
                                    if ((B * x + tmp9 + i + k) < 0 || (B * x + tmp9 + i + k) > (N - 1) ||
                                        (B * y + tmp9 + j + l) < 0 || (B * y + tmp9 + j + l) > (M - 1)) {
                                        p2 = 0;
                                    } else {
                                        p2 = previous[(B * x + tmp9 + i + k) * M + B * y + tmp9 + j + l];
                                    }


                                    dist += abs(p1 - p2);

                                }
                            if (dist < min) {
                                min = dist;
                                bestx = i;
                                besty = j;
                            }
                        }
                    // d=d/2;

                    vectors_x[x * M_B + y] += bestx;
                    vectors_y[x * M_B + y] += besty;
                } else if (dd == 2) {
                    //d=1;
                    min = 255 * B * B;

                    for (i = -1; i < 1 + 1; i += 1)      /* For all candidate blocks */
                        for (j = -1; j < 1 + 1; j += 1) {
                            dist = 0;
                            for (k = 0; k < B; k++)        /* For all pixels in the block */
                                for (l = 0; l < B; l++) {
                                    int tmp9 = vectors_x[x * M_B + y];
                                    p1 = current[(B * x + k) * M + B * y + l];
                                    if ((B * x + tmp9 + i + k) < 0 || (B * x + tmp9 + i + k) > (N - 1) ||
                                        (B * y + tmp9 + j + l) < 0 || (B * y + tmp9 + j + l) > (M - 1)) {
                                        p2 = 0;
                                    } else {
                                        p2 = previous[(B * x + tmp9 + i + k) * M + B * y + tmp9 + j + l];
                                    }

                                    dist += abs(p1 - p2);

                                }
                            if (dist < min) {
                                min = dist;
                                bestx = i;
                                besty = j;
                            }
                        }
                    // d=d/2;

                    vectors_x[x * M_B + y] += bestx;
                    vectors_y[x * M_B + y] += besty;
                }
            //ok
        }
}

