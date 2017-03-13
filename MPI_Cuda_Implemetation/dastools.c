#include <stdio.h>
#include <math.h>

#define MAX_GRID_SIZE 65535

#include <math.h>
#include <string.h>    //for strncpy
#include <stdlib.h>
#include <time.h>
#include <unistd.h>    //for close

typedef unsigned char uint8;


// Dastools Version 012


void zero_array_int(int *array, int xdim, int ydim) {
    int x, y;
    for (y = 0; y < ydim; y++)
        for (x = 0; x < xdim; x++)
            array[y * xdim + x] = 0;
}


void zero_array_uint8(uint8 *array, int xdim, int ydim) {
    int x, y;
    for (x = 0; x < xdim; x++)
        for (y = 0; y < ydim; y++)
            array[x * ydim + x] = 0;
}

void print_array(unsigned char *array, int xdim, int ydim) {
    int x, y;
    for (y = 0; y < ydim; y++)
        for (x = 0; x < xdim; x++)
            printf("array (x,y)=(%d,%d)[%d]=%d\n", x, y, y * xdim + x, (unsigned char) array[y * xdim + x]);
}


void print_array_int(int *array, int xdim, int ydim) {
    int x, y;
    for (y = 0; y < ydim; y++)
        for (x = 0; x < xdim; x++)
            printf("array (x,y)=(%d,%d)[%d]=%d\n", x, y, y * xdim + x, (int) array[y * xdim + x]);
}


void print_array_int_formated(int *array, int xdim, int ydim) {
    int x, y;
    for (y = 0; y < ydim; y++) {
        for (x = 0; x < xdim; x++) { printf(" \t(%d,%d)[%d]=%d ", x, y, y * xdim + x, (int) array[y * xdim + x]); }
        printf("\n");
    }
}


#ifdef _OPENMPI
#include <mpi.h>  //for MPI Calls
void mpiDebug(int error) {
    int eclass,len;
    char estring[MPI_MAX_ERROR_STRING];
    if( error!=0 ) {
        MPI_Error_class(error, &eclass);
        MPI_Error_string(error, estring, &len);
        printf("Code: Error %d: %d %s\n", error, eclass, estring);fflush(stdout);
        exit(error);
    }
}  


void mpiDebugHelper(void) {
    int i = 0;
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    printf("******** PID %d on %s ready for attach (gdb: set var i = 7 ) *******\n", getpid(), hostname);
    fflush(stdout);
    while (0 == i)
        sleep(5);
}
#endif


void compute_work_effort(int maxjobs, int nprocs, int *array) {
    int i;
    //array is a two dimensional array, like x[1][processes]

    //Every process gets chunksize lines
    int chunksize = (maxjobs / nprocs);

    int restwork = maxjobs % nprocs;
    int j = 0;
    printf("ComputeWorkEffor: TotalProcesses: %d TotalWork %d ", nprocs, maxjobs);

    if (restwork != 0) {
        printf(" and remain to be distributed %d.\n", restwork);
        // a fix required
        for (i = 0; i < nprocs; i++) {
            //x starts at previous line
            array[i * 2 + 0] = j;
            // ends at previous line plus chunksize + upoloipo
            j = j + chunksize;

            if (restwork > 0) {
                restwork--;
                j++;
                array[i * 2 + 1] = j;
            } else {
                array[i * 2 + 1] = j;
            }
        }
    } else {
        //no fix is required
        //we compute the start line of every node, and the finish one
        printf(" and nothing remain to be distributed.\n");
        for (i = 0; i < nprocs; i++) {
            array[i * 2 + 0] = j;
            j = j + chunksize;
            array[i * 2 + 1] = j;
        }
    }

}


#ifdef CUDA
#include <cuda.h>
#include <cuda_runtime_api.h>
#include <builtin_types.h>
#include "globals.ch"
void checkCUDAError(const char *msg) {
  cudaError_t error = cudaGetLastError();
  if ( error != cudaSuccess) {
     fprintf(stderr, "Cuda error: %s: %s.\n", msg, cudaGetErrorString(error) );
     exit(EXIT_FAILURE);
  }
}





void gpu_print_info()
{
int dev_count;
int i;
int maxBlockSize;

#ifdef __cplusplus
  cudaDeviceProp dev_prop;
#else // !_cplusplus
  struct cudaDeviceProp dev_prop;
#endif

   // ######### GET INFO ABOUT GPU ###########################################
   /* Count available devices */
   cudaGetDeviceCount(&dev_count);

   if(dev_count>0) {
       printf("\nfound %d device(s)",dev_count);

       /* For every device */
       for(i=0; i<dev_count; i++) {
           /* Get device properties */
           cudaGetDeviceProperties(&dev_prop, i);

           printf("\nDevice(%d) characteristics\n",i);
           printf("-------------------------\n");
           printf("GPU: %s\n",dev_prop.name);
           printf("Memory: %.0f Mbytes\n", ceil( (float)dev_prop.totalGlobalMem/(1024*1024)));
           //printf("Multiprocessors: %d, Cores/MP: %d \n", dev_prop.multiProcessorCount, _ConvertSMVer2Cores(dev_prop.major, dev_prop.minor));
           printf("Multiprocessors: %d\n", dev_prop.multiProcessorCount);
           printf("Clock: %.2f Mhz\n", (float)dev_prop.clockRate/(1000*1000));
           maxBlockSize = dev_prop.maxThreadsPerBlock;
           printf("Threads/block: %d Max\n", maxBlockSize );
           printf("Blocks/Grid: %d Max\n", MAX_GRID_SIZE );
           printf("Compute Capability: %d.%d\n",dev_prop.major,dev_prop.minor);
           printf("-------------------------\n");
           fflush(NULL);
       }
   }
   else {
       printf("\nNo CUDA device found!\n");
       exit(-1);
   }

}


void cudaDebug(cudaError_t error) {
           if( error!=0 ) {
                       printf("Code: %d, %s\n", error, cudaGetErrorString( error ) );
                               exit(error);
                                       }
}

#endif
