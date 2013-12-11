#ifndef _WEMAT_
#define _WEMAT_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Error values definitions
#define WEMAT_ERR_ARG -1		//Argument Error: Is returned when a parameter is null (the most common motive) or invalid
#define WEMAT_ERR_MEM -2		//Memory Error: Is returned when the memory allocation fails internaly in a function
#define WEMAT_ERR_DIM -3		//Dimentions Error: Is returned wheb you pass matrix and the dimesions don't martch (like sum)
#define WEMAT_ERR_FILE -4		//File Read/Write Error: Is returned when a file is not correctly processed (the most common usse is the name) 
#define WEMAT_ERR_TRUNCATE -5     //File truncate during read, the dimentions of the file may be wrong.

/*
 The matrix must be supplied in Column Major Order
 |1 2 3|
 |4 5 6|
 =[1 4 2 5 3 6]
*/

//matrix utility functions for 4x4 matrix float type
void	wematPrint(float *A);
void	wematTranslateScale(float x,float y, float z, float s, float *m);
void    wematAddScale(float sx,float sy,float sz, float *m);
void	wematAddTranslate(float x,float y, float z, float *m);
int	wematRotate(float polar,float azimutal, float *m);
void	wematIdentity(float *m);
void	wematMult(float *A, float *B, float *C);
int	wematInverse(float *A, float *result);
int	wematODEtoGL(float *rot,float *pos,float *m16);
int	wematTransponse(float *A, float *result);
int	wematOrtho(float left, float right, float bottom, float top, float nearVal, float farVal, float *m);
int     wematMultVector(float *m, float *vec4, float *res4);


#endif

