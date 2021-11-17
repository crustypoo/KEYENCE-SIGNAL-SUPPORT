/*
 * fft_support.h
 *
 * 		Created on: August 10, 2015
 * 		    Author: Andy Liu
 *	  Organization: N12 Technologies
 *
 *	  	   Summary: A header file
 */

#ifndef FFT_SUPPORT_H_
#define FFT_SUPPORT_H_

#include "support.h"

/*
 * function: fft_init
 * purpose: fftw3 initialization functions, allocates arrays of type double and fftw_complex
 * 			of width: OUT_NUM and length: FFT_BUFFER or FFT_HALFBUFF
 * returns: 0 - success, -1 - failure
 */
int fft_init() {
	// iterate through OUT_NUM
	int i;
	for (i = 0; i < OUT_NUM; i++) {
		// Allocate memory for the input and output buffers
		IN[i] = (double *) malloc(sizeof(double) * FFT_BUFFER);
		OUT[i] = (fftw_complex *) fftw_malloc(
				sizeof(fftw_complex) * FFT_HALFBUFF);

		if (IN[i] == NULL || OUT[i] == NULL ) {
			printf("Error: fft_init failed mem allocation on %d array!\n", i);
			return -1;
		}

		// initialize buffers to 0
		memset(IN[i], 0, FFT_BUFFER);
		memset(OUT[i], 0, FFT_HALFBUFF);

		// generate plan for FFTW 3, exhaustive mode.
		p[i] = fftw_plan_dft_r2c_1d(FFT_BUFFER, IN[i], OUT[i], FFTW_EXHAUSTIVE);
		if (p[i] == NULL ) {
			printf("ERROR: fft_init failed to generate %d plans for fftw!\n",
					(OUT_NUM - i));
			return -1;
		}
	}
	return 0;
}

/*
 * function: power_buff
 * purpose: allocates memory for the power spectrum buffer of type sig_type.
 * returns: 0 - success, -1 - failure
 */
int PB_alloc() {
	PB = (sig_type *) malloc(sizeof(sig_type) * FFT_HALFBUFF);
	int err = (PB != NULL ) ? 0 : -1;
	return err;
}

/*
 * function: detect_amplitude
 * purpose: calculates the amplitude of the sinusoidal components of a signal.
 * 			Applies a FFT algorithm first to deduce the real and img amplitude
 * 			components of the signal.
 * returns: 0 - success, -1 - failure
 */
int detect_amplitude() {
	int k;
	for (k = 0; k < OUT_NUM; k++) {
		fftw_execute(p[k]);
	}

	int i = 0;
	int j = 0;
	for (i = 0; i < FFT_HALFBUFF; i++) {
		for (j = 0; j < OUT_NUM; j++) {
			double re = *(OUT[j] + i)[0];
			double im = *(OUT[j] + i)[1];

			*(PB + i)[j] = sqrt((re * re) + (im * im));
		}
	}
	if (i != (FFT_HALFBUFF - 1) && j != (OUT_NUM - 1))
		return -1;
	else
		return 0;
}

/*
 * Min/Max functions for radix 2 arrays
 * (WORK IN PROGRESS)
 */
double * Max(double *array, int N) {
	if (N % 2 != 0) {
		return array;
	} else {
		double array2[N / 2];
		int k = 0;
		int imax;
		for (imax = 0; imax < N / 2; imax += 2) {
			array2[k] =
					(array[imax] >= array[imax + 1]) ?
							array[imax] : array[imax + 1];
			k++;
		}
		return Max(array2, N / 2);
	}
}

double * Min(double *array, int N) {
	if (N % 2 != 0) {
		return array;
	} else {
		double array2[N / 2];
		int k;
		int imin;
		for (imin = 0; imin < N / 2; imin += 2) {
			array2[k] =
					(array[imin] <= array[imin + 1]) ?
							array[imin] : array[imin + 1];
			k++;
		}
		return Min(array2, N / 2);
	}
}

#endif /* FFT_SUPPORT */
