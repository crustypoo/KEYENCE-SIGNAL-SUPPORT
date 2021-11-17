/*
 * filter_support.h
 *
 * 	 	Created on: Aug 7, 2015
 *  	    Author: Andy Liu
 *	  Organization: N12 Technologies
 *
 *	  	   Summary: A header file with support functions for implementing a windowed FIR filter
 *
 */

#ifndef FILTER_SUPPORT_H_
#define FILTER_SUPPORT_H_

#include "support.h"

/*
 * function: coeff_alloc
 * purpose: Allocates memory for the filter coefficients
 * returns: 0 - success, -1 - failure
 */
int coeff_alloc() {
	double * W = (double *) malloc(sizeof(double) * BUFFER_LEN);
	F = (double *) malloc(sizeof(double) * BUFFER_LEN);

	memset(W, 0, BUFFER_LEN);
	memset(F, 0, BUFFER_LEN);

	int err = (W != NULL && F != NULL ) ? 0 : -1;

	return err;
} /* int coeff_alloc */

/*
 * function: buff_alloc
 * purpose: Allocates memory for the running buffer
 * returns: 0 - success, -1 - failure
 */
int FB_alloc() {
	FB = (sig_type *) malloc(sizeof(sig_type) * BUFFER_LEN);
	int err = (FB != NULL ) ? 0 : -1;
	return err;
} /* int FB_alloc */

/*
 * function: window_coeffs
 * purpose: calculates window coefficients
 * returns: 0 -success, -1 - failure
 * Codes for int wintype: 0: Hanning, 1: Hamming, 2: Blackman, 3: Blackman-Harris
 */
int window_coeffs(int wintype) {

	if (wintype == 0) {
		/*
		 * Hanning Window Coefficients
		 */
		double alpha = 0.5;

		int i;
		for (i = 0; i <= BUFFER_LEN; i++) {
			double x0 = (2.0 * pi * i) / BUFFER_LEN;
			double x1 = cos(x0);
			*(W + i) = alpha - alpha * x1;
		}
		return 0;
	} else if (wintype == 1) {
		/*
		 * Hamming Window Coefficients
		 */
		double alpha = 0.54;
		double beta = 0.46;

		int i;
		for (i = 0; i <= BUFFER_LEN; i++) {
			*(W + i) = alpha - beta * cos((2 * pi * i) / BUFFER_LEN);
		}
		return 0;
	} else if (wintype == 2) {
		/*
		 * Blackman Window Coefficients
		 */
		double alpha = 0.16;
		double a0 = (1 - alpha) / 2;
		double a1 = .5;
		double a2 = alpha / 2;

		int i;
		for (i = 0; i <= BUFFER_LEN; i++) {
			*(W + i) = a0 - a1 * cos((2 * pi * i) / BUFFER_LEN)\

					+ a2 * cos((4 * pi * i) / BUFFER_LEN);
		}
		return 0;
	} else if (wintype == 3) {
		/*
		 * Blackman-Harris Window Coefficients
		 */
		double a0 = 0.35875;
		double a1 = 0.48829;
		double a2 = 0.14128;
		double a3 = 0.01168;

		int i;
		for (i = 0; i <= BUFFER_LEN; i++) {
			*(W + i) = a0 - a1 * cos((2 * pi * i) / BUFFER_LEN)\

					+ a2 * cos((4 * pi * i) / BUFFER_LEN)\

					- a3 * cos((6 * pi * i) / BUFFER_LEN);
		}
		return 0;
	} else {
		printf("Error: unidentified window type\n");
		return -1;
	}

} /* int window_coeffs */

/*
 * function: filt_coeffs
 * purpose: calculates the coefficients for a windowed FIR filter
 * returns: 0 - success, -1 - failure
 * codes:
 * 		int win_type: 0: Hanning, 1: Hamming, 2: Blackman, 3: Blackman Harris
 * 		int filt_type: 0: low-pass, 1: high-pass, 2: bandpass
 */
int filt_coeffs(double F_LOW, double F_HIGH, double FS, int win_type,
		int filt_type) {

	int err = coeff_alloc(BUFFER_LEN);
	if (err == -1) {
		printf("Error: filt_coeffs failed to allocate buffers\n");
		return err;
	}

	double M = BUFFER_LEN / 2;
	double omega_c1 = (2 * pi * F_LOW) / FS;
	double omega_c2 = (2 * pi * F_HIGH) / FS;
	int hd;

	// compute window coefficients
	int w;
	w = window_coeffs(win_type);

	// check if window coefficients were properly calculated
	if (w != -1) {
		if (filt_type == 0) {
			// Low-Pass filter
			int i;
			for (i = 0; i <= BUFFER_LEN; i++) {
				if (M != i) {
					hd = sin((omega_c1 * (i - M))) / (pi * (i - M));
					*(F + i) = *(W + i) * hd;
				} else {
					hd = omega_c1 / pi;
					*(F + i) = *(W + i) * hd;
				}
			}
			free(W);
			return 0;
		} else if (filt_type == 1) {
			// High-Pass filter
			int i;
			for (i = 0; i <= BUFFER_LEN; i++) {
				if (M != i) {
					hd = 1 - (omega_c1 / pi);
					*(F + i) = *(W + i) * hd;
				} else {
					hd = -sin(omega_c1 * (i - M)) / (pi * (i - M));
					*(F + i) = *(W + i) * hd;
				}
			}
			free(W);
			return 0;
		} else if (filt_type == 2) {
			// Band-Pass filter
			int i;
			for (i = 0; i <= BUFFER_LEN; i++) {
				if (M != i) {
					hd = (sin(omega_c2 * (i - M)) / (pi * (i - M)))\

							- (sin(omega_c1 * (i - M)) / (pi * (i - M)));
					*(F + i) = *(W + i) * hd;
				} else {
					hd = (omega_c2 - omega_c1) / pi;
					*(F + i) = *(W + i) * hd;
				}
			}
			free(W);
			return 0;
		}
		// filter code was unspecified type
		else {
			free(F);
			free(W);
			printf("Error: filt_coeffs failed, undf filt code!\n");
			return -1;
		}
	}
	// window_coeffs failed
	else {
		free(W);
		free(F);
		printf("Error: filt_coeffs failed at window_coeffs!\n");
		return -1;
	}
} /*int filt_coeffs*/

#endif /* FILTER_SUPPORT_H_ */
