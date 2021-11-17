/*
 * fft_support.c
 *
 *   Created on: Aug 24, 2015
 *       Author: Andy Liu
 * Organization: N12 Technologies
 */

#include "sig_support.c"

/*
 * function: fft_init
 * purpose: fftw3 initialization functions, allocates arrays of type double and fftw_complex
 * 			of width: OUT_NUM and length: FFT_BUFFER or FFT_HALFBUFF
 * inputs: - double * IN[]
 * 		   - fftw_complex * OUT[]
 * 		   - fftw_plan plan[]
 * 		   - int FFT_BUFFER
 * returns: 0 - success, -1 - failure
 */
int fft_init(double * in[], fftw_complex * out[], fftw_plan p[], int fft_buffer) {
	int FFT_HALFBUFF = (fft_buffer / 2) + 1;

	// iterate through OUT_NUM
	int i;
	for (i = 0; i < OUT_NUM; i++) {
		// Allocate memory for the input and output buffers
		*(in + i) = (double *) malloc(sizeof(double) * fft_buffer);
		*(out + i) = (fftw_complex *) fftw_malloc(
				sizeof(fftw_complex) * FFT_HALFBUFF);

		if (in[i] == NULL || out[i] == NULL ) {
			printf("Error: fft_init failed mem allocation on %d array!\n", i);
			return -1;
		}

		// initialize buffers to 0
		memset(in[i], 0, fft_buffer);
		memset(out[i], 0, FFT_HALFBUFF);

		// generate plan for FFTW 3, exhaustive mode.
		p[i] = fftw_plan_dft_r2c_1d(fft_buffer, in[i], out[i], FFTW_EXHAUSTIVE);
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
 * inputs: - sig_type * PB
 * 		   - int FFT_HALFBUFF
 * returns: 0 - success, -1 - failure
 */
int PB_alloc(sig_type * pb, int FFT_HALFBUFF) {
	pb = (sig_type *) malloc(sizeof(sig_type) * FFT_HALFBUFF);
	int err = (pb != NULL ) ? 0 : -1;
	return err;
}

/*
 * function: detect_amplitude
 * purpose: calculates the amplitude of the sinusoidal components of a signal.
 * 			Applies a FFT algorithm first to deduce the real and img amplitude
 * 			components of the signal.
 * inputs: - fftw_plan p[]
 * 		   - fftw_complex * OUT[]
 * 		   - sig_type * PB
 * 		   - int FFT_HALFBUFF
 * returns: 0 - success, -1 - failure
 */
int detect_amplitude(fftw_plan p[], fftw_complex * out[], sig_type * pb, int fft_halfbuff) {
	int k;
	for (k = 0; k < OUT_NUM; k++) {
		fftw_execute(p[k]);
	}

	int i = 0;
	int j = 0;
	for (i = 0; i < fft_halfbuff; i++) {
		for (j = 0; j < OUT_NUM; j++) {
			double re = *(out[j] + i)[0];
			double im = *(out[j] + i)[1];

			*(pb + i)[j] = sqrt((re * re) + (im * im));
		}
	}
	if (i != (fft_halfbuff - 1) && j != (OUT_NUM - 1))
		return -1;
	else
		return 0;
}

/*
 * Min/Max functions for radix 2 arrays
 * (WORK IN PROGRESS)
 */
double * Max(double *array, int n) {
	if (n % 2 != 0) {
		return array;
	} else {
		double array2[n / 2];
		int k = 0;
		int imax;
		for (imax = 0; imax < n / 2; imax += 2) {
			array2[k] =
					(array[imax] >= array[imax + 1]) ?
							array[imax] : array[imax + 1];
			k++;
		}
		return Max(array2, n / 2);
	}
}

double * Min(double *array, int n) {
	if (n % 2 != 0) {
		return array;
	} else {
		double array2[n / 2];
		int k;
		int imin;
		for (imin = 0; imin < n / 2; imin += 2) {
			array2[k] =
					(array[imin] <= array[imin + 1]) ?
							array[imin] : array[imin + 1];
			k++;
		}
		return Min(array2, n / 2);
	}
}
