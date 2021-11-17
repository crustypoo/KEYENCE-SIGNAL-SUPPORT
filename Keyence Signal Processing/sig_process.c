/*
 * sig_process.c
 *
 *  	Created on: Aug 14, 2015
 *      	Author: Andy Liu
 *    Organization: N12 Technologies
 */

#include "filter_support.h"
#include "fft_support.h"

/*
 * function: init_all
 * purpose: Initialization routine for the keyence signal processing.
 * 			- Allocates memory for buffers
 * 			- Calculates filter coefficients
 * 			- Executes plan optimization for FFTW3
 * functions called: - int coeff_alloc()
 * 					 - int PB_alloc()
 * 					 - int FB_alloc()
 * 					 - int fft_init();
 * 					 - int filt_coeffs(double FL, double FH, double FS, int win_type, int filt_type);
 *
 * returns: 0 - success, -1 - failure
 */
int init_all(){
	printf("init_all begin .");

	/*
	 * allocate memory for buffers
	 */
	// allocate for filter coefficients
	int err = coeff_alloc();
	if (err == -1){
		printf("Error: init_all error - coeff_alloc failed!");
		return -1;
	} printf(" .");

	// allocate for filter running buffer
	err = FB_alloc();
	if (err == -1){
		printf("Error: init_all error - FB_alloc failed!");
		return -1;
	} printf(" .");

	// allocate for power spectrum buffers
	err = PB_alloc();
	if (err == -1){
		printf("Error: init_all error - PB_alloc failed!");
		return -1;
	} printf(" .");

	/*
	 * initializations for fftw3 library
	 */
	err = fft_init();
	if (err == -1){
		printf("Error: init_all error - fft_init failed!");
		return -1;
	} printf(" .");

	/*
	 * calculate the filter coefficients
	 */
	err = filt_coeffs(FL, FH, SR, BLACKHARRIS, LOWPASS);
	if (err == -1){
		printf("Error: init_all error - filt_coeffs failed!");
		return -1;
	} printf(" .\n");

	printf("init_all successful!\n");
	return 0;

} /* int init_all */

/*
 * function: shift_buffer
 * purpose: shifts the buffer of interest left or right
 * returns: 0 - success, -1 - failure
 */
int shift_buffer(int dir, double * dp, sig_type * sp, int * ip, float * fp, char * cp, int len){
	/*
	 * Shift left
	 */
	if (dir == 1){
		if(dp == NULL){
			if(sp == NULL){
				if(ip == NULL){
					if(fp == NULL){
						if (cp == NULL){
							printf("Error: shift_buffer could not resolve buffer pointer!");
							return -1;
						}else{
							memmove((cp + 1), cp, sizeof(char) * (len - 1));
						} // cp check
					}else{
						memmove((fp + 1), fp, sizeof(float) * (len - 1));
					} // fp check
				}else{
					memmove((ip + 1), ip, sizeof(int) * (len - 1));
				} // ip check
			}else{
				memmove((sp + 1), sp, sizeof(sig_type) * (len - 1));
			} // sp check
		}else{
			memmove((dp + 1), dp, sizeof(double) * (len - 1));
		} // dp check
	// Shift left
	}else if (dir == -1){
		if(dp == NULL){
			if(sp == NULL){
				if(ip == NULL){
					if(fp == NULL){
						if (cp == NULL){
							printf("Error: shift_buffer could not resolve buffer pointer!");
							return -1;
						}else{
							memmove(cp, (cp + 1), sizeof(char) * (len - 1));
						} // cp check
					}else{
						memmove(fp, (fp + 1), sizeof(float) * (len - 1));
					} // fp check
				}else{
					memmove(ip, (ip + 1), sizeof(int) * (len - 1));
				} // ip check
			}else{
				memmove(sp, (sp + 1), sizeof(sig_type) * (len - 1));
			} // sp check
		}else{
			memmove(dp, (dp + 1), sizeof(double) * (len - 1));
		} // dp check
	}else{
		printf("Error: shift_buffer could not resolve direction of shift!");
		return -1;
	}
	printf("success\n");
	return 0;
}/* int shift_buffer */

/*
 * function: filter_process
 * purpose: performs a convolution of the input signal.
 * returns: 0 - success, -1 - failure
 *
 * functions called: - int shift_buffer()
 */
int filter_process(double *input){
	int err = 0;
	err = shift_buffer(-1, NULL, FB,  NULL, NULL, NULL, BUFFER_LEN);
	if (err == -1)
		return err;

	int i;
	for(i = 0; i < OUT_NUM; i++)
		*(FB + BUFFER_LEN - 1)[i] = *(input + i);
	int j;
	for (i = 0; i < BUFFER_LEN; i++){
		for (j = 0; j < OUT_NUM; j++){
			filt_output[j] = filt_output[j] + (F[i] * (*(FB + i)[j]));
		}
	}
	return err;
} /* int filter_process */

int spectral_process(){
	int err = 0;
	err = shift_buffer(1)
}

int main(){
 init_all();
 shift_buffer(-1, NULL, FB,  NULL, NULL, NULL, BUFFER_LEN);
 exit(0);
}


