/*
 * filter_support.c
 *
 *   Created on: Aug 24, 2015
 *       Author: Andy Liu
 * Organization: N12 Technologies
 *
 */

#include "sig_support.c"

/*
 * function: coeff_alloc
 * purpose: Allocates memory for the filter coefficients
 * inputs: - double * W
 * 		   - double * F
 * 		   - int BUFFER_LEN
 * returns: 0 - success, -1 - failure
 */
int coeff_alloc(double * w, double * f, int buffer_len) {
	w = (double *) malloc(sizeof(double) * buffer_len);
	f = (double *) malloc(sizeof(double) * buffer_len);

	memset(w, 0, buffer_len);
	memset(f, 0, buffer_len);

	int err = (w != NULL && f != NULL ) ? 0 : -1;

	return err;
} /* int coeff_alloc */

/*
 * function: buff_alloc
 * purpose: Allocates memory for the running buffer
 * inputs: - sig_type * FB
 * 		   - int BUFFER_LEN
 * returns: 0 - success, -1 - failure
 */
int FB_alloc(sig_type * fb, int buffer_len) {
	fb = (sig_type *) malloc(sizeof(sig_type) * buffer_len);
	int err = (fb != NULL ) ? 0 : -1;
	return err;
} /* int FB_alloc */

/*
 * function: window_coeffs
 * purpose: calculates window coefficients
 * inputs: - int wintype
 *         - double * W
 *         - int BUFFER_LEN
 * returns: 0 -success, -1 - failure
 * Codes for int wintype: 0: Hanning, 1: Hamming, 2: Blackman, 3: Blackman-Harris
 */
int window_coeffs(int wintype, double * w, int buffer_len) {

	if (wintype == 0) {
		/*
		 * Hanning Window Coefficients
		 */
		double alpha = 0.5;

		int i;
		for (i = 0; i <= buffer_len; i++) {
			double x0 = (2.0 * pi * i) / buffer_len;
			double x1 = cos(x0);
			*(w + i) = alpha - alpha * x1;
		}
		return 0;
	} else if (wintype == 1) {
		/*
		 * Hamming Window Coefficients
		 */
		double alpha = 0.54;
		double beta = 0.46;

		int i;
		for (i = 0; i <= buffer_len; i++) {
			*(w + i) = alpha - beta * cos((2 * pi * i) / buffer_len);
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
		for (i = 0; i <= buffer_len; i++) {
			*(w + i) = a0 - a1 * cos((2 * pi * i) / buffer_len)\

					+ a2 * cos((4 * pi * i) / buffer_len);
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
		for (i = 0; i <= buffer_len; i++) {
			*(w + i) = a0 - a1 * cos((2 * pi * i) / buffer_len)\

					+ a2 * cos((4 * pi * i) / buffer_len)\

					- a3 * cos((6 * pi * i) / buffer_len);
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
		int filt_type, double * w, double * f, int buffer_len) {

	int err = coeff_alloc(w, f, buffer_len);
	if (err == -1) {
		printf("Error: filt_coeffs failed to allocate buffers\n");
		return err;
	}

	double M = buffer_len / 2;
	double omega_c1 = (2 * pi * f_low) / fs;
	double omega_c2 = (2 * pi * f_high) / fs;
	int hd;

	// compute window coefficients
	int a;
	a = window_coeffs(win_type, w, buffer_len);

	// check if window coefficients were properly calculated
	if (a != -1) {
		if (filt_type == 0) {
			// Low-Pass filter
			int i;
			for (i = 0; i <= buffer_len; i++) {
				if (M != i) {
					hd = sin((omega_c1 * (i - M))) / (pi * (i - M));
					*(f + i) = *(w + i) * hd;
				} else {
					hd = omega_c1 / pi;
					*(f + i) = *(w + i) * hd;
				}
			}
			free(w);
			return 0;
		} else if (filt_type == 1) {
			// High-Pass filter
			int i;
			for (i = 0; i <= buffer_len; i++) {
				if (M != i) {
					hd = 1 - (omega_c1 / pi);
					*(f + i) = *(w + i) * hd;
				} else {
					hd = -sin(omega_c1 * (i - M)) / (pi * (i - M));
					*(f + i) = *(w + i) * hd;
				}
			}
			free(w);
			return 0;
		} else if (filt_type == 2) {
			// Band-Pass filter
			int i;
			for (i = 0; i <= buffer_len; i++) {
				if (M != i) {
					hd = (sin(omega_c2 * (i - M)) / (pi * (i - M)))\

							- (sin(omega_c1 * (i - M)) / (pi * (i - M)));
					*(f + i) = *(w + i) * hd;
				} else {
					hd = (omega_c2 - omega_c1) / pi;
					*(f + i) = *(w + i) * hd;
				}
			}
			free(w);
			return 0;
		}
		// filter code was unspecified type
		else {
			free(f);
			free(w);
			printf("Error: filt_coeffs failed, undf filt code!\n");
			return -1;
		}
	}
	// window_coeffs failed
	else {
		free(w);
		free(f);
		printf("Error: filt_coeffs failed at window_coeffs!\n");
		return -1;
	}
} /*int filt_coeffs*/
