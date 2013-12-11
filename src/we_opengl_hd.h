#ifndef _WE_SPLASH_
#define _WE_SPLASH_

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

#endif

