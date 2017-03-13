// Minas Dasygenis HS- reloaded
// 2014-02
// All functions divided to individual files for better management
// 

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>


int main(argc, argv)
        int argc;
        char *argv[];
{


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
    // printf("Hello world from processor %s, rank %d"
    //               " out of %d processors\n",
    //       processor_name, world_rank, world_size);
#pragma omp parallel
    {
        printf("Hello world from processor %s, rank %d"
                       " out of %d processors, Thread %d\n",
               processor_name, world_rank, world_size, omp_get_thread_num());
    }


    MPI_Finalize();


    return 0;
}
