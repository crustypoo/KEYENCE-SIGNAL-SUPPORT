/*
 * filter_helper.h
 *
 *  Created on: Aug 24, 2015
 *      Author: aliu
 */

#ifndef FILTER_HELPER_H_
#define FILTER_HELPER_H_

/*
 * function: coeff_alloc
 * purpose: Allocates memory for the filter coefficients
 * inputs: - double * W
 * 		   - double * F
 * 		   - int BUFFER_LEN
 * returns: 0 - success, -1 - failure
 */
int coeff_alloc(double * w, double * f, int buffer_len);

/*
 * function: buff_alloc
 * purpose: Allocates memory for the running buffer
 * inputs: - sig_type * FB
 * 		   - int BUFFER_LEN
 * returns: 0 - success, -1 - failure
 */
int FB_alloc(sig_type * fb, int buffer_len);

/*
 * function: window_coeffs
 * purpose: calculates window coefficients
 * inputs: - int wintype
 *         - double * W
 *         - int BUFFER_LEN
 * returns: 0 -success, -1 - failure
 * Codes for int wintype: 0: Hanning, 1: Hamming, 2: Blackman, 3: Blackman-Harris
 */
int window_coeffs(int wintype, double * w, int buffer_len);

/*
 * function: filt_coeffs
 * purpose: calculates the coefficients for a windowed FIR filter
 * inputs: - double F_LOW
 *         - double F_HIGH
 *         - double FS
 *         - int win_type
 *         - int filt_type
 *         - double * W
 *         - double * F
 *         - int BUFFER_LEN
 * returns: 0 - success, -1 - failure
 * codes:
 * 		int win_type: 0: Hanning, 1: Hamming, 2: Blackman, 3: Blackman Harris
 * 		int filt_type: 0: low-pass, 1: high-pass, 2: bandpass
 */
int filt_coeffs(double f_low, double f_high, double fs, int win_type,
		int filt_type, double * w, double * f, int buffer_len);

#endif /* FILTER_HELPER_H_ */
