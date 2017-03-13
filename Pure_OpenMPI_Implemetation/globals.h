#include <stdio.h>

/* All global definitions go here */
typedef unsigned char uint8;


/* Dimesions NxM, B block size and p search area */
int M;
int N;
int B;
int p;
int nframes;    //Number of frames to check
int sframe;        //Starting Frame
char *filename;


// We define these as globals in order to use them in successive HS calls and
// not malloc and free in every iteration
uint8 *currentframe, *previousframe, *outputframe;
int *motion_vectors_x, *motion_vectors_y;


//Define N_B and M_B macros
int N_B;
int M_B;


/* Define Function Prototypes */
void readframe(FILE *fp, uint8 framed[N][M], int framenumber, int xdimension, int ydimension);

void writeframe(uint8 framed[N][M]);

double SNR(uint8 current[N][M], uint8 output[N][M]);

void
motion_compensation(uint8 previous[N][M], int vectors_x[N / B][M / B], int vectors_y[N / B][M / B], uint8 output[N][M]);

void fs_motion_estimation(uint8 current[N][M], uint8 previous[N][M], int vectors_x[N / B][M / B],
                          int vectors_y[N / B][M / B]);

FILE *opensequence(char *filename);

void log_motion_estimation(uint8 *current, uint8 *previous, int *vectors_x, int *vectors_y);


/* Define Function Prototypes */
void readframe(FILE *fp, uint8 framed[N][M], int framenumber, int xdimension, int ydimension);

void readframe2(FILE *fp, uint8 *framed, int framenumber, int xdimension, int ydimension);

void read_sequence(char current[N][M], char previous[N][M]);

double SNR2(uint8 *current, uint8 *output);

void motion_compensation2(uint8 *previous, int *vectors_x, int *vectors_y, uint8 *output);

FILE *opensequence(char *filename);

void fs_motion_estimation2(uint8 *current, uint8 *previous, int *vectors_x, int *vectors_y);

double SNRFRAME(uint8 current[N][M], uint8 output[N][M]);


void writeframe2(uint8 *framed);

void writeframe(uint8 framed[N][M]);

double SNR(uint8 current[N][M], uint8 output[N][M]);

void
motion_compensationFRAME(uint8 previous[N][M], int vectors_x[N_B][M_B], int vectors_y[N_B][M_B], uint8 output[N][M]);

void zero_array_uint8(uint8 *array, int xdim, int ydim);

void zero_array_int(int *array, int xdim, int ydim);
