/*
 *  tiir.h
 *  IIRFilter
 *
 *  Created by José Federico  Ramos Ortega on 12/11/12.
 *  Copyright 2012 Facultad De Ingeniería Electronica. All rights reserved.
 *
 */

#ifndef _LIIR_
#define _LIIR_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LIIR_END_INPUT	-10
#define LIIR_ERR_MEM	-11
#define LIIR_ERR_OPEN	-12
#define LIIR_ERR_READ   -13

typedef struct LIIRTag {
	double *b;
	double *a;
	int Co;
	double *input;
	double *output;
	double *v;
	int lenghtInput;
	int	n;
}LIIR;


int	LIIRInit(LIIR *filter, double *b, double *a, int lenghtCoef, int lenghtInput);
int LIIRFree(LIIR *filter);
int	LIIRFilter(LIIR *filter, double input, double *output);
int	LIIRFilter2(LIIR *filter, double input, double *output);
int LIIRRewind(LIIR *filter);
int LIIRFileRead(LIIR *filter,char *filename);
int LIIRFileWrite(LIIR *filter, char *filename);

void filter(int ord, double *a, double *b, int np, double *x, double *y);


#endif