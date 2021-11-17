/*
 * support.h
 *
 *  	Created on: Aug 10, 2015
 *          Author: Andy Liu
 *    Organization: N12 Technologies
 *
 *         Summary: Support files for keyence signal processing package
 */

#ifndef SUPPORT_H_
#define SUPPORT_H_



// libraries to be included
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fftw3.h>

/* FILTER and WINDOW TYPES */
// window types
#define HANNING 0
#define HAMMING 1
#define BLACKMAN 2
#define BLACKHARRIS 3
// filter types
#define LOWPASS 0
#define HIGHPASS 1
#define BANDPASS 2

// System setting constants
#define BUFFER_LEN 40
#define FFT_BUFFER 1024
#define FFT_HALFBUFF 513
#define OUT_NUM 3
#define pi  3.14159265358979323846264338327950

// define new type called sig_type (multi dimensional array)
typedef double sig_type[OUT_NUM];

/* FILTER SETTINGS */
// Low Frequency Cutoff
const double FL = 0.001;
// High Frequency Cutoff
const double FH = 0.0;
// Sampling Rate
const double SR = 200.0;

// Pointers for filter coeff buffers
double *W;
double *F;

// Pointers for FFTW buffers
double *IN[OUT_NUM];
fftw_complex *OUT[OUT_NUM];

// Pointer to FFTW plan
fftw_plan p[OUT_NUM];

// Pointer to running buffers
// running filter buffer
sig_type *FB;

// power spectrum buffer
sig_type *PB;

// out array of len = OUT_NUM
double filt_output[OUT_NUM];

/*
 * function: array_match
 * purpose: function to match character arrays
 * returns: 0 - success, -1 - failure
 */
int array_match(char * sub, char * match, int m_len){
	int i;
	for(i = 0; i < m_len; i++){
		if (*(sub + i) != *(match + i)){
			return -1;
		}
	}
	return 0;
}

#endif /* SUPPORT_H_ */
