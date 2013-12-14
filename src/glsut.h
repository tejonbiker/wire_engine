#ifndef _GLSUT_
#define _GLSUT_

/* A simple lib to load shaders(vertex and fragment), linking and querying from compiled program
*/

#ifdef  GLPI
	#include "GLES2/gl2.h"
	#include <stdbool.h>

	#warning "GLES Pi Headers in use"
#else
	#warning "Mac Headers not used"
#endif

#ifdef __APPLE__
	#include <OpenGL/OpenGL.h>
	#warning "Mac OS OpenGL Headers in use"
#else
	#warning "Pi headers not used"
#endif

#ifdef _WIN32


    #define GLEW_STATIC
    #include <windows.h>
    #include <GL/glew.h>
    #include <GL/glut.h>
    #include<GL/gl.h>
    #include<GL/glu.h>
    #include<GL/glext.h>

    //typedef char  GLchar;
    #warning "Windows 32 bits detected"
#endif

#ifdef _WIN64
    #include<GL\gl.h>
    #include<GL\glu.h>
    #include<GL\glext.h>
    typedef char  GLchar;
    #warning "Windows 32 bits detected"
#endif

#include <stdio.h>
#include <string.h>

enum{
	SUT_ERR_ARG=-1,
	SUT_ERR_MEM=-2,
	SUT_ERR_DIM=-3,
	SUT_ERR_FILE=-4,
};

#define GLPL_MAX_SHADER_SIZE 5000


#warning "Header included: glsut"
typedef struct BaseShaderTag{
	GLint vertex_shader;
	GLint fragment_shader;
	GLint program;
}BaseShader;

int  glsutLoadShaders(FILE *messages, BaseShader *info_shader, char *fragment_file, char *vertex_file);
//Similar to glplotLoadShader but can recibe from char
int glsutLoadShadersChar(FILE *messages,BaseShader *info_shader,const  char *fragment_src, const char *vertex_src);

int	glsutCheckLocations(int *base_loc,int nvars);

/*
int glsutLinkUniform(BaseShader *info_shader,GLint *uniform);
int glsutLinkAttrib(BaseShader *infor_shader,GLint *attrib);
*/

#endif
