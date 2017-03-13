// Minas Dasygenis HS- reloaded
// 2014-02
// All functions divided to individual files for better management
// 

#include <stdio.h>
#include <math.h>
#include <string.h>    //for strncpy
#include <stdlib.h>
#include <time.h>
#include <unistd.h>    //for close
#include <mpi.h>

#include "globals.h"

#include "3slog_cuda.h"


// Cleanup function
void exit_function(void) {
    printf("***Exiting Program***\n");
    free(filename);
    free(currentframe);
    free(previousframe);
    free(outputframe);
    free(motion_vectors_x);
    free(motion_vectors_y);
    freeMemory();
}


int main(argc, argv)
        int argc;
        char *argv[];
{

/*N=1;
M=1;
B=1;
p=1;
sframe=1;
nframes=1;
previousframe=0;
currentframe=0;
outputframe=0;
current2=0;
previous2=0;
current4=0;
previous4=0;
*/

    MPI_Init(&argc, &argv);


    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d"
                   " out of %d processors\n", processor_name, world_rank, world_size);

//The first step is to parse all command line parameters
    if (argc < 7) {
        printf("7 variables are required: N M B p startingframe number_frames_to_process sequence\n");
        printf("Example: ./main 720 576 16 7 0 1 barb.yuv\n");
        exit(-3);
    }


//Register the exit function
    atexit(*exit_function);


//We will use this for our filename
//256 character name are enough...(I think)
    filename = (char *) malloc(256 * sizeof(char));


    N = atoi(argv[1]);
    M = atoi(argv[2]);
    B = atoi(argv[3]);
    p = atoi(argv[4]);
    sframe = atoi(argv[5]);
    nframes = atoi(argv[6]);
    strncpy(filename, argv[7], 255);

//Define here the N/B and M/B variables
    N_B = (int) N / B;
    M_B = (int) M / B;

//Check if we have a full division
    if (N % B != 0) {
        printf("Warning: N Not fully divided. Fixing it\n");
        N_B++;
    }
    if (M % B != 0) {
        printf("Warning: M Not fully divided. Fixing it\n");
        M_B++;
    }

    printf("Arguments: N=%d, M=%d, B=%d, p=%d, sframe=%d, nframes=%d, sequence=%s, N_B=%d, M_B=%d\n", N, M, B, p,
           sframe, nframes, filename, N_B, M_B);



    // Pointer to the file of the video sequences
    FILE *picturefp;

    int i; // i frame

    struct timespec start, end; // The time variables

    //double snr = 0.0;


    picturefp = opensequence(filename);

    //Do a memory allocation for the used arrays

    currentframe = (uint8 *) calloc((N + 1) * (M + 1) * sizeof(int), 1);
    if (currentframe == NULL) {
        perror("Memory cannot be allocated to current");
        exit(-2);
    }
    previousframe = (uint8 *) calloc((N + 1) * (M + 1) * sizeof(int), 1);
    if (previousframe == NULL) {
        perror("Memory cannot be allocated to previous");
        exit(-2);
    }
    outputframe = (uint8 *) calloc((N + 1) * (M + 1) * sizeof(int), 1);
    if (outputframe == NULL) {
        perror("Memory cannot be allocated to output");
        exit(-2);
    }
    motion_vectors_x = (int *) calloc((N_B) * (M_B) * sizeof(int), 1);
    if (motion_vectors_x == NULL) {
        perror("Memory cannot be allocated to output");
        exit(-2);
    }
    motion_vectors_y = (int *) calloc((N_B) * (M_B) * sizeof(int), 1);
    if (motion_vectors_y == NULL) {
        perror("Memory cannot be allocated to output");
        exit(-2);
    }
//
//    int* motion_vectors_x2 = (int *) calloc((N_B) * (M_B) * sizeof(int), 1);
//    if (motion_vectors_x == NULL) {
//        perror("Memory cannot be allocated to output");
//        exit(-2);
//    }
//    int* motion_vectors_y2 = (int *) calloc((N_B) * (M_B) * sizeof(int), 1);
//    if (motion_vectors_y == NULL) {
//        perror("Memory cannot be allocated to output");
//        exit(-2);
//    }

    int nDevices;

    cudaGetDeviceCount(&nDevices);
    if (nDevices > 0) {
        printf("\n %d Device found at", nDevices);
        printf(" processor %s, rank %d"
                       " out of %d processors\n", processor_name, world_rank, world_size);
        initValuesAndAllocateMemory(M_B, N_B, B, M, N);

        cudaThreads = 280;
    }


    if (world_rank == 0) {
        //Time measurement
        clock_gettime(CLOCK_MONOTONIC, &start);
    }


    for (i = sframe; i < nframes + sframe; i++) {

        if (i % world_size != world_rank) {
            continue;
        }

        readframe2(picturefp, currentframe, i, N, M);
        readframe2(picturefp, previousframe, i - 1, N, M);


        //zero_array_uint8(outputframe,N+1,M+1);
        if (nDevices > 0) {
            initKernelAndStartIt(currentframe, previousframe, motion_vectors_x, motion_vectors_y);
//
        } else {
            log_motion_estimation(currentframe, previousframe, motion_vectors_x, motion_vectors_y);
        }
        printf(".");
        //Clear the buffer to print the dot
        fflush(NULL);
    }

    MPI_Barrier(MPI_COMM_WORLD);
// END of check
    if (world_rank == 0) {
//Time measurement

        clock_gettime(CLOCK_MONOTONIC, &end);

        const int DAS_NANO_SECONDS_IN_SEC = 1000000000;
        long timeElapsed_s = end.tv_sec - start.tv_sec;
        long timeElapsed_n = end.tv_nsec - start.tv_nsec;
        if (timeElapsed_n < 0) {
            timeElapsed_n = DAS_NANO_SECONDS_IN_SEC + timeElapsed_n;
            timeElapsed_s--;
        }
        printf("\n processor %s, rank %d"
                       " out of %d processors", processor_name, world_rank, world_size);
        printf("\tTime: %ld.%09ld secs \n", timeElapsed_s, timeElapsed_n);
    }

    MPI_Finalize();

    //for test uncomment the following
    //zero_array_int(motion_vectors_x,N_B,M_B);
    //zero_array_int(motion_vectors_y,N_B,M_B);

    //old: static frame based
    //motion_compensationFRAME(previousframe,motion_vectors_x,motion_vectors_y,outputframe);
    //snr=SNRFRAME(currentframe,outputframe);

    //New with pointers
    // motion_compensation2(previousframe, motion_vectors_x, motion_vectors_y, outputframe);
    //snr = SNR2(currentframe, outputframe);
    // writeframe2(outputframe);
    // printf("SNR = %f\n", snr);


    return 0;
}
