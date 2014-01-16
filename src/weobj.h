#ifndef _WEOBJ_
#define _WEOBJ_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "we_opengl_hd.h"
#include "weobject.h"

#define OBJ_BUFFER_LENGHT 256

/*
typedef struct tagWEShaderVars{
	GLint vertex_attrib;
	GLint color_attrib;
	GLint normal_attrib;
	GLint texture_attrib;
	GLint sampler;
	GLint modelview;
	GLint program_id;
}WEShaderVars;
*/

typedef struct tagWEObjMat{
}WEObjMat;

typedef struct tagWEObjModel{
	float nFaces;
	WEFace *faces;
}WEObjModel;


int weObjLoad(FILE *outmsg,char *filename,WEObjModel *model);
int weObjDelete(WEObjModel *model);
int weObjDraw(WEObjModel *model, WEShaderVars *shader_vars, float *mv);

#endif

