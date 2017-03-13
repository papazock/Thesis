#include "globals.h"

#include <stdio.h> 	// For FILE construct
#include <stdlib.h> //	for exit
#include <string.h> // for strcat

//Version 14

FILE *opensequence(char *filename) {
    FILE *picturefp;

    // We open the file here
    if ((picturefp = fopen(filename, "rb")) == NULL) {
        printf("video sequence %s doesn't exist\n", filename);
        exit(-1);
    }
    return picturefp;
}


/* This function reads a frame from file pointer which is already opened */
/* Inputs: filepointer
 * 		   framed array to put the frame
 * 		   xdimension 
 * 		   y dimension
 * 		   number of the frame to be read
 *
 * 		   I assume that this is a yuv file 4:2:0, and we only care
 * 		   for the Y information (neglecting the two C?? )
 * 		   */
void readframe2(FILE *fp, uint8 *framed, int framenumber, int xdimension, int ydimension) {
    int temp;
    int i, j;
    int seekframe;
    long int offset;


    //Every frame occupies the same ammount of space:
    //frame= width x height + width x height / 4 + width x height / 4
    seekframe = (int) (xdimension * ydimension * 3) / 2;
    offset = seekframe * framenumber;

    fseek(fp, offset, 0);

    for (i = 0; i < xdimension; i++) {
        for (j = 0; j < ydimension; j++) {
            temp = 1.164 * (fgetc(fp) - 16);
            if (temp > 255) { temp = 255; }
            if (i >= N || j >= M) { printf("i=%d,j=%d\n", i, j); }
            framed[i * M + j] = temp;
        }

    }

}


void writeframe2(uint8 *framed) {
    FILE *picture;
    int i, j;
    char *filenameW;
    char tempspace[9];
    filenameW = (char *) malloc(128 * sizeof(char));


    strcpy(filenameW, "dasygenisTEST_");
    sprintf(tempspace, "%d", N);
    strcat(filenameW, tempspace);
    strcat(filenameW, "x");
    sprintf(tempspace, "%d", M);
    strcat(filenameW, tempspace);
    strcat(filenameW, ".raw");

    if ((picture = fopen(filenameW, "w")) == NULL) {
        printf("Cannot write video sequence\n");
        exit(-1);
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            fputc(framed[i * M + j], picture);
        }
    }
    printf("Frame %s was extracted and saved\n", filenameW);
    fclose(picture);
    free(filenameW);
}



/* This function reads a frame from file pointer which is already opened */
/* Inputs: filepointer
 *                 framed array to put the frame
 *                 xdimension 
 *                 y dimension
 *                 number of the frame to be read
 *
 *                 I assume that this is a yuv file 4:2:0, and we only care
 *                 for the Y information (neglecting the two C?? )
 *                 */

void readframe(FILE *fp, uint8 framed[N][M], int framenumber, int xdimension, int ydimension) {
    int temp;
    int i, j;
    int seekframe;
    long int offset;


    //Every frame occupies the same ammount of space:
    //frame= width x height + width x height / 4 + width x height / 4
    seekframe = (int) (xdimension * ydimension * 3) / 2;
    offset = seekframe * framenumber;

    fseek(fp, offset, 0);


    for (i = 0; i < xdimension; i++) {
        for (j = 0; j < ydimension; j++) {
            temp = 1.164 * (fgetc(fp) - 16);
            if (temp > 255) { temp = 255; }
            framed[i][j] = temp;
        }

    }

}


void writeframe(uint8 framed[N][M]) {
    FILE *picture;
    int i, j;
    char *filenameW;
    char tempspace[9];
    filenameW = (char *) malloc(128 * sizeof(char));


    strcpy(filenameW, "dasygenisTEST_");
    sprintf(tempspace, "%d", N);
    strcat(filenameW, tempspace);
    strcat(filenameW, "x");
    sprintf(tempspace, "%d", M);
    strcat(filenameW, tempspace);
    strcat(filenameW, ".raw");

    if ((picture = fopen(filenameW, "w")) == NULL) {
        printf("Cannot write video sequence\n");
        exit(-1);
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            fputc(framed[i][j], picture);
        }
    }
    printf("Frame %s was extracted and saved\n", filenameW);
    fclose(picture);
    free(filenameW);
}


void read_sequenceFRAME(char current[N][M], char previous[N][M]) {
    FILE *infile;
    int i, j;
    puts("Reading 1st Frame frame0.yuv");
    if ((infile = fopen("frame0.yuv", "rb")) == NULL) {
        printf("sequence doesn't exist\n");
        exit(-1);
    }
    for (i = 0; i < N; i++)
        for (j = 0; j < M; j++)
            previous[i][j] = fgetc(infile);

    fclose(infile);
    puts("Reading 2nd Frame frame1.yuv");
    if ((infile = fopen("frame1.yuv", "rb")) == NULL) {
        printf("sequence doesn't exist\n");
        exit(-1);
    }

    for (i = 0; i < N; i++)
        for (j = 0; j < M; j++)
            current[i][j] = fgetc(infile);

    fclose(infile);
}

