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


/*
	Setup a IIR estructure, the argument b and a are the coefficients of the difference direct form
	Equation, lenghtCoef is the lenght of the array of coeficients
*/
int	LIIRInit(LIIR *filter, double *b, double *a, int lenghtCoef, int lenghtInput);
int 	LIIRFree(LIIR *filter); 				//Liberate the resources of the filter
int	LIIRFilter(LIIR *filter, double input, double *output); //Apply filter (don't use)
int	LIIRFilter2(LIIR *filter, double input, double *output); //Apply filter, infinite inputs
int 	LIIRRewind(LIIR *filter);				//Reset the before values
int 	LIIRFileRead(LIIR *filter,char *filename);		//Read a file to load coefficients
int 	LIIRFileWrite(LIIR *filter, char *filename);		//Write a file with the coefficients
int	LIIRMatlabRead(LIIR *filter,char *filename); 		//Read MATLAB cfc file exported from fdatool

//Is the reference of the library, the inspiration of this code
void filter(int ord, double *a, double *b, int np, double *x, double *y);


#endif