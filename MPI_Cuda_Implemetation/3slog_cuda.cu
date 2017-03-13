#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "3slog_cuda.h"
}

__global__ void log_motion_estimation_cuda(uint8 *current, uint8 *previous, int *vectors_x, int *vectors_y,
                                           int *M_B, int *N_B, int *B, int *M, int *N) {
    //obtain idx;
    int id = blockIdx.x * blockDim.x + threadIdx.x;
    if (id >= ((*M_B) * (*N_B))) return;
    int x, y;

    x = id / (*M_B);
    y = id % (*M_B);


    int dd;
    for (dd = 4; dd > 1; dd--) {  //--> gives 4 3 2
        int step = 0;
        if (dd == 4) {
            //d=4;
            step = 4;
        } else if (dd == 3) {
            step = 2;
        } else if (dd == 2) {
            step = 1;
        } else {
            continue;
        }

        int min = 255 * (*B) * (*B);
        int bestx, besty, i, j, k, l;
        for (i = -step; i < step + 1; i += step)      /* For all candidate blocks */
            for (j = -step; j < step + 1; j += step) {
                int dist = 0;
                for (k = 0; k < (*B); k++)        /* For all pixels in the block */
                    for (l = 0; l < (*B); l++) {
                        int tmp9 = vectors_x[x * (*M_B) + y];
                        int p1, p2;
                        p1 = current[((*B) * x + k) * (*M) + (*B) * y + l];
                        if (((*B) * x + tmp9 + i + k) < 0 || ((*B) * x + tmp9 + i + k) > ((*N) - 1) ||
                            ((*B) * y + tmp9 + j + l) < 0 || ((*B) * y + tmp9 + j + l) > ((*M) - 1)) {
                            p2 = 0;
                        } else {
                            p2 = previous[((*B) * x + tmp9 + i + k) * (*M) + (*B) * y + tmp9 + j + l];
                        }

                        dist += abs(p1 - p2);
                    }
                if (dist < min) {
                    min = dist;
                    bestx = i;
                    besty = j;
                }
            }

        int at = x * (*M_B) + y;

        vectors_x[at] += bestx;

        vectors_y[at] += besty;

    }
}


__global__ void log_motion_estimation_cuda2(uint8 *current, uint8 *previous, int *vectors_x, int *vectors_y,
                                            int M_B, int N_B, int B, int M, int N) {
    //obtain id;
    int id = blockIdx.x * blockDim.x + threadIdx.x;

    if (id >= (M_B * N_B)) {
        //printf("id=%d \t M_B*N_B=%d \t \n", id, M_B*N_B);
        return;
    }
    int x, y;
    x = id / M_B;
    y = id % M_B;
    vectors_x[id] = 0;
    vectors_y[id] = 0;
//    if(id==8023||id==8091||id==8159) {
//        printf("id=%d \t x=%d \t y=%d \n", id,x,y);
//    }
//
//    y=id;
//    x=0;
//    while(y>=M_B){
//        y=y-M_B;
//        x++;
//    }

    int dd;
    for (dd = 4; dd > 1; dd--) {  //--> gives 4 3 2
        int step = 0;
        if (dd == 4) {
            //d=4;
            step = 4;
        } else if (dd == 3) {
            step = 2;
        } else if (dd == 2) {
            step = 1;
        } else {
            continue;
        }

        int min = 255 * B * B;
        int bestx, besty, i, j, k, l;
        for (i = -step; i < step + 1; i += step)      /* For all candidate blocks */
            for (j = -step; j < step + 1; j += step) {
                int dist = 0;
                for (k = 0; k < B; k++)        /* For all pixels in the block */
                    for (l = 0; l < B; l++) {
                        int tmp9 = vectors_x[id];
                        int p1, p2;
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


        vectors_x[id] += bestx;

        vectors_y[id] += besty;

    }
}

extern "C"
void make_zeros(int *vectors_x, int *vectors_y, int N_B, int M_B) {
    for (int c = 0; c < N_B; c++)
        for (int b = 0; b < M_B; b++) {
            vectors_x[c * M_B + b] = 0;
            vectors_y[c * M_B + b] = 0;
        }
}

extern "C"
void initKernelAndStartIt(uint8 *current, uint8 *previous, int *vectors_x, int *vectors_y) {
    cudaError_t err = cudaSuccess;
    //make_zeros(vectors_x, vectors_y, t_N_B, t_M_B);

    if ((cudaMemcpy(d_current, current, framesize, cudaMemcpyHostToDevice) != cudaSuccess) ||
        (cudaMemcpy(d_previous, previous, framesize, cudaMemcpyHostToDevice) != cudaSuccess) ||
        (cudaMemcpy(d_vectors_x, vectors_x, sizeof(int) * totalsize, cudaMemcpyHostToDevice) != cudaSuccess) ||
        (cudaMemcpy(d_vectors_y, vectors_y, sizeof(int) * totalsize, cudaMemcpyHostToDevice) != cudaSuccess)) {
        err = cudaGetLastError();
        if (err != cudaSuccess) {
            printf("Failed to memcpy (error code: %s)!\n", cudaGetErrorString(err));
        }
    }

    //log_motion_estimation_cuda <<< (int)ceil(totalsize / cudaThreads) , cudaThreads >>>(d_current, d_previous, d_vectors_x, d_vectors_y, d_M_B, d_N_B, d_B, d_M, d_N);
    log_motion_estimation_cuda2 <<< totalsize / cudaThreads + 1, cudaThreads >>>
                                                                  (d_current, d_previous, d_vectors_x, d_vectors_y, t_M_B, t_N_B, t_B, t_M, t_N);

    err = cudaGetLastError();
    if (err != cudaSuccess) {
        printf("Failed to launch add kernel (error code: %s)!\n", cudaGetErrorString(err));
    }

    cudaDeviceSynchronize();
    if ((cudaMemcpy(vectors_x, d_vectors_x, sizeof(int) * totalsize, cudaMemcpyDeviceToHost) != cudaSuccess) ||
        (cudaMemcpy(vectors_y, d_vectors_y, sizeof(int) * totalsize, cudaMemcpyDeviceToHost) != cudaSuccess)) {
        err = cudaGetLastError();
        if (err != cudaSuccess) {
            printf("Failed to memcpy (error code: %s)!\n", cudaGetErrorString(err));
        }
    }

}



extern "C"
void initValuesAndAllocateMemory(int M_B, int N_B, int B, int M, int N) {
    totalsize = (N_B) * (M_B);
    t_N_B = N_B;
    t_M_B = M_B;
    t_B = B;
    t_M = M;
    t_N = N;
    framesize = sizeof(int) * (N + 1) * (M + 1);


    d_current = (uint8 *) fixed_cudaMalloc(framesize);
    d_previous = (uint8 *) fixed_cudaMalloc(framesize);
    d_vectors_y = (int *) fixed_cudaMalloc(totalsize * sizeof(int));
    d_vectors_x = (int *) fixed_cudaMalloc(totalsize * sizeof(int));

    if ((cudaMalloc(&d_M_B, sizeof(int)) != cudaSuccess) ||
        (cudaMalloc(&d_N_B, sizeof(int)) != cudaSuccess) ||
        (cudaMalloc(&d_B, sizeof(int)) != cudaSuccess) ||
        (cudaMalloc(&d_M, sizeof(int)) != cudaSuccess) ||
        (cudaMalloc(&d_N, sizeof(int)) != cudaSuccess)) {

    }

    if ((cudaMemcpy(d_M_B, &M_B, sizeof(int), cudaMemcpyHostToDevice) != cudaSuccess) ||
        (cudaMemcpy(d_N_B, &N_B, sizeof(int), cudaMemcpyHostToDevice) != cudaSuccess) ||
        (cudaMemcpy(d_B, &B, sizeof(int), cudaMemcpyHostToDevice) != cudaSuccess) ||
        (cudaMemcpy(d_M, &M, sizeof(int), cudaMemcpyHostToDevice) != cudaSuccess) ||
        (cudaMemcpy(d_N, &N, sizeof(int), cudaMemcpyHostToDevice) != cudaSuccess)) {

    }


}
extern "C"
void freeMemory() {
    cudaFree(d_vectors_y);
    cudaFree(d_vectors_x);
    cudaFree(d_current);
    cudaFree(d_previous);
    cudaFree(d_M);
    cudaFree(d_N);
    cudaFree(d_B);
    cudaFree(d_M_B);
    cudaFree(d_N_B);

}

extern "C"
void *fixed_cudaMalloc(size_t len) {
    void *p;
    if (cudaMalloc(&p, len) == cudaSuccess) return p;
    return 0;
}
