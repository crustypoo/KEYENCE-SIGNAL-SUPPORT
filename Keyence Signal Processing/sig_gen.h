/*
 * sig_gen.h
 *
 *    Created on: Aug 14, 2015
 *        Author: Andy Liu
 * 	Organization: N12 technologies
 *
 * 		 Summary: header containing signal generation functions for test purposes
 */

#ifndef SIG_GEN_H_
#define SIG_GEN_H_

#include "support.h"

	double * sine_gen(double amp, double freq, double N, int start, int end){
		static double wave[N];
		double delta = (end - start) / N;
		double k = start;
		for (int i = 0; i< N; i++){
			wave[i] = amp * sin(2 * pi * freq * k);
			k = k + (i * delta);
		}
		return wave;
	}


#endif /* SIG_GEN_H_ */
