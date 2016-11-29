// by Chris Papazoglou

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "motionEstARPS.h"
#include "motionEstFullSearch.h"


int main(int argc, char **argv) {


    srand(time(NULL));
    int **A = createVectorTest(352, 352);
    int **B = createVectorTest(352, 352);
    printf("start of Full Search\n");
    fflush(NULL);
    TheResult *re = motionEstES(A, B, 16, 7, 352, 352);

    printf("Full Search computations: %d\n", re->computations);
    fflush(NULL);

//    printf("start of ARPS\n");
//    fflush(NULL);
//    TheResult *re2 = motionEstARPS(A, B, 16, 7, 352, 352);
//
//    printf("ARPS computations: %d\n", re2->computations);
//    fflush(NULL);
//
//    int i=0;
//    for(i=0;i<re->mvCol;i++){
//        printf("i= %d re1[x,y]= re1[0,1]=[%d, %d]\n",i,re->motionVect[0][i],re->motionVect[1][i]);
//        printf("i= %d re2[x,y]= re2[0,1]=[%d, %d]\n",i, re2->motionVect[0][i],re2->motionVect[1][i]);
//    }

    return 0;

}
