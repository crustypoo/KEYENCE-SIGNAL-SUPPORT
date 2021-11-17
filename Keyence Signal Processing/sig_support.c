/*
 * sig_support.c
 *
 *   Created on: Aug 24, 2015
 *       Author: Andy Liu
 * Organization: N12 Technologies
 */

// libraries to be included
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fftw3.h>

#define OUT_NUM 3
#define pi  3.14159265358979323846264338327950

// define new type called sig_type (multi dimensional array)
typedef double sig_type[OUT_NUM];
