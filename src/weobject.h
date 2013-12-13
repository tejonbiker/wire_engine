#ifndef _WEOBJECT_
#define _WEOBJECT_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "we_opengl_hd.h"

typedef struct tagWEVec3{
	float x,y,z;
}WEVect3;

typedef struct tagWEPoint{
	float vertex[3];
	float color[3];
	float texcoord[2];
	float normal[3];
}WEPoint;

typedef struct tagWEQuad{
float 	vertex[12];
float 	color[12];
float 	normal[12];
float 	texture[8];
GLushort	indices[6];
int 	texture_id;
}WEQuad;

typedef struct tagWEShaderVars{
	GLint vertex_attrib;
	GLint color_attrib;
	GLint normal_attrib;
	GLint texture_attrib;
	GLint sampler;
	GLint modelview;
}WEShaderVars;

int  weQuadCreate(WEQuad *object,float *color,int texture_id); 	//Create a quad with normal outside the screen
int  weQuadCoordTex(float *ct,WEQuad *quad);		      	//Setup the texture coords of an Quad

int  weVect3FromFile(char *filename,WEVect3 **ptrDest,int *n); //Read points from file, in n is the total points readed
int  weVect3Draw(WEVect3 *array, int nPoints ,WEShaderVars *shader_vars, float *color, float *mv);
int  wePointDraw(WEPoint *array,int nPoints,WEShaderVars *shader_vars, float *mv);

#endif

