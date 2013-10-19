#ifndef __GLPI__
#define __GLPI__

/*Simple framework to link OpenGL in Raspberry Pi.
 Don't forget include the headers and libs in your makefile

 FLAGS = -I/opt/vc/include/
 LIBS= -lGLESv2 -lEGL -lm -lbcm_host -L/opt/vc/lib

*/

#include "bcm_host.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES2/gl2.h"
#include <stdio.h>

#include <stdio.h> // sprintf
#include <stdlib.h>  // malloc
#include <math.h>
#include <fcntl.h>  // open fcntl
#include <unistd.h> // read close
#include <dirent.h>  // scandir
#include <string.h> // strlen
#include "linux/kd.h"	// keyboard stuff...
#include "termios.h"
#include "sys/ioctl.h"
#include <linux/input.h>

typedef struct ScreenDataTag{
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	uint32_t width;
	uint32_t height;
}ScreenData;

static struct termios tty_attr_old;
static int old_keyboard_mode;

static int __mouse_fd=-1;

#define dflag(x) printf("Flag: %i\n",x);

int glpiInit();
int glpiEnd();
int glpiSwapBuffers();

void	glpiDoEvents();
int     glpiGetWindowSize(int *x, int *y);
int*	glpiGetMouseArray();
char*	glpiGetKeysArray();
void  	glpiCloseInput();


#endif
