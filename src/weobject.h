#ifndef _WEOBJECT_
#define _WEOBJECT_

#include <stdio.h>
#include <string.h>
#include "we_opengl_hd.h"


typedef struct tagWEQuad{
float 	vertex[12];
float 	color[12];
float 	normal[12];
float 	texture[8];
GLushort	indices[6];
int 	texture_id;
}WEQuad;

int  weQuadCreate(WEQuad *object,float *color,int texture_id); //Create a quad with normal outside the screen
int  weQuadDelete(WEQuad *object);


#endif

