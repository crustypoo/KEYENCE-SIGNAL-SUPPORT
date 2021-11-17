/*
 * fft_helper.h
 *
 *  Created on: Aug 24, 2015
 *      Author: aliu
 */

#ifndef FFT_HELPER_H_
#define FFT_HELPER_H_

#include "fft_helper.c"

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
int fft_init(double * in[], fftw_complex * out[], fftw_plan p[], int fft_buffer);

/*
 * function: power_buff
 * purpose: allocates memory for the power spectrum buffer of type sig_type.
 * inputs: - sig_type * PB
 * 		   - int FFT_HALFBUFF
 * returns: 0 - success, -1 - failure
 */
int PB_alloc(sig_type * pb, int FFT_HALFBUFF);

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
int detect_amplitude(fftw_plan p[], fftw_complex * out[], sig_type * pb, int fft_halfbuff);



#endif /* FFT_HELPER_H_ */
