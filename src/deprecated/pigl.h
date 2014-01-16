#ifndef GLPI
#define GLPI

/*Simple framework to link OpenGL in Raspberry Pi.
 Don't forget include the headers and libs in your makefile
 
 FLAGS = -I/opt/vc/include/
 LIBS= -lGLESv2 -lEGL -lm -lbcm_host -L/opt/vc/lib
 
*/

#include "bcm_host.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES2/gl2.h"

typedef struct ScreenDataTag{
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	uint32_t width;
	uint32_t height;
}ScreenData;

int glpiInit();
int glpiEnd();


#endif