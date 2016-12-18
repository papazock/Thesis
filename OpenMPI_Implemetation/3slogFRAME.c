/* 3 step logarithmic search with motion compensation and SNR computation
   Minas Dasygenis 2013
   Compile it with gcc 3slog.c  -lm */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define TT 2
#define N 1920
#define M 1088
#define B 16
#define p 15
#define r 7
#define a 3

void log_motion_estimationFRAME(char current[N][M], char previous[N][M], char vectors_x[N / B][M / B],
                                char vectors_y[N / B][M / B]) {
    int x, y, i, j, k, l, p1, p2, dist, dd, min, c, b, bestx, besty;
//int d;
    puts("log_ME Started");
    for (c = 0; c < N / B; c++)
        for (b = 0; b < M / B; b++) {
            vectors_x[c][b] = 0;
            vectors_y[c][b] = 0;
        }

    for (x = 0; x < N / B; x++)            /* For all blocks in the current frame */
        for (y = 0; y < M / B; y++) {
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
                                    int tmp9 = vectors_x[x][y];
                                    p1 = current[B * x + k][B * y + l];
                                    if ((B * x + tmp9 + i + k) < 0 || (B * x + tmp9 + i + k) > (N - 1) ||
                                        (B * y + tmp9 + j + l) < 0 || (B * y + tmp9 + j + l) > (M - 1)) {
                                        p2 = 0;
                                    } else
                                        p2 = previous[B * x + tmp9 + i + k][B * y + tmp9 + j + l];


                                    dist += abs(p1 - p2);

                                }
                            if (dist < min) {
                                min = dist;
                                bestx = i;
                                besty = j;
                            }
                        }
                    // d=d/2;

                    vectors_x[x][y] += bestx;
                    vectors_y[x][y] += besty;
                } else if (dd == 3) {
                    //d=2;
                    min = 255 * B * B;

                    for (i = -2; i < 2 + 1; i += 2)      /* For all candidate blocks */
                        for (j = -2; j < 2 + 1; j += 2) {
                            dist = 0;
                            for (k = 0; k < B; k++)        /* For all pixels in the block */
                                for (l = 0; l < B; l++) {
                                    int tmp9 = vectors_x[x][y];
                                    p1 = current[B * x + k][B * y + l];
                                    if ((B * x + tmp9 + i + k) < 0 || (B * x + tmp9 + i + k) > (N - 1) ||
                                        (B * y + tmp9 + j + l) < 0 || (B * y + tmp9 + j + l) > (M - 1)) {
                                        p2 = 0;
                                    } else
                                        p2 = previous[B * x + tmp9 + i + k][B * y + tmp9 + j + l];


                                    dist += abs(p1 - p2);

                                }
                            if (dist < min) {
                                min = dist;
                                bestx = i;
                                besty = j;
                            }
                        }
                    // d=d/2;

                    vectors_x[x][y] += bestx;
                    vectors_y[x][y] += besty;
                } else if (dd == 2) {
                    //d=1;
                    min = 255 * B * B;

                    for (i = -1; i < 1 + 1; i += 1)      /* For all candidate blocks */
                        for (j = -1; j < 1 + 1; j += 1) {
                            dist = 0;
                            for (k = 0; k < B; k++)        /* For all pixels in the block */
                                for (l = 0; l < B; l++) {
                                    int tmp9 = vectors_x[x][y];
                                    p1 = current[B * x + k][B * y + l];
                                    if ((B * x + tmp9 + i + k) < 0 || (B * x + tmp9 + i + k) > (N - 1) ||
                                        (B * y + tmp9 + j + l) < 0 || (B * y + tmp9 + j + l) > (M - 1)) {
                                        p2 = 0;
                                    } else
                                        p2 = previous[B * x + tmp9 + i + k][B * y + tmp9 + j + l];


                                    dist += abs(p1 - p2);

                                }
                            if (dist < min) {
                                min = dist;
                                bestx = i;
                                besty = j;
                            }
                        }
                    // d=d/2;

                    vectors_x[x][y] += bestx;
                    vectors_y[x][y] += besty;
                }
            //ok
        }
}
