// Minas Dasygenis  -- extract a frame from a sequence
// 2014-02
// All functions divided to individual files for better management
// 

#include <stdio.h>
#include <math.h>
#include <string.h>	//for strncpy
#include <stdlib.h>
#include <time.h>

#include "globals.h"


// Cleanup function
void exit_function(void) {
    printf("***Exiting Program***\n");
    free(filename);
}


int main(argc, argv)
        int argc;
        char *argv[];
{



//The first step is to parse all command line parameters
    if (argc < 5) {
        printf("Extracting a frame from 4:4:0. Minas Dasygenis,2014\n");
        printf("4 variables are required: N M frameNR sequence\n");
        printf("Example: ./main 720 576 10 barb.yuv\n");
        exit(-3);
    }


//Register the exit function
    atexit(*exit_function);


//We will use this for our filename
//256 character name are enough...(I think)
    filename = (char *) malloc(256 * sizeof(char));


    N = atoi(argv[1]);
    M = atoi(argv[2]);
    sframe = atoi(argv[3]);
    strncpy(filename, argv[4], 255);

    printf("Arguments: N=%d, M=%d, sframe=%d, sequence=%s\n", N, M, sframe, filename);



    // Definitions of current and previous frames
    uint8 currentframe[N][M];

    // Pointer to the file of the video sequences
    FILE *picturefp;


    picturefp = opensequence(filename);
    readframe(picturefp, currentframe, sframe, N, M);
    writeframe(currentframe);
    close(picturefp);


    return 1;
}

