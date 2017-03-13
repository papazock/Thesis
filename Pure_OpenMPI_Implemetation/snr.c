#include <math.h>
#include "globals.h"

double SNR2(uint8 *current, uint8 *output) {
    double msd = 0.0, psnr;
    int i, j;

    for (i = 0; i < N; i++)
        for (j = 0; j < M; j++)
            //msd+=(double)((current[i][j]-output[i][j])*(current[i][j]-output[i][j]));
            msd += (double) ((current[i * M + j] - output[i * M + j]) * (current[i * M + j] - output[i * M + j]));

    psnr = 10 * log10((float) N * M * 255 * 255 / msd);

    return psnr;
}


double SNRFRAME(uint8 current[N][M], uint8 output[N][M]) {
    double msd = 0.0, psnr;
    int i, j;

    for (i = 0; i < N; i++)
        for (j = 0; j < M; j++)
            msd += (double) ((current[i][j] - output[i][j]) * (current[i][j] - output[i][j]));

    psnr = 10 * log10((float) N * M * 255 * 255 / msd);

    return psnr;
} 

