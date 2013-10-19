/*
 	 A little lib to grab images from OpenGL framebuffer and write to an image,
	 ideally yo work with headless mode of the raspberry pi.

	Dont forget link with -lSOIL in Raspbian

	Warning: Only save in BMP
*/

#ifndef _SOIL_UTIL_
#include <SOIL/SOIL.h>
#include <stdio.h>
#include "bcm_host.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES2/gl2.h"



/**
       * Grab and save the image from the framebuffer
       * @param filename a string containing the full filename
       * @return 0 if all are correct, -1 if we have and error
*/
int suGrabAndSave(char *filename);

/**
       * Set the basename for a continuos image writing is desired.
       * @param name_base a string contaigint the base name
       * @return 0 if all are correct, -1 if we have and error
*/
int suSetBaseName(char *name_base);

/**
       * Grab the image from Framebuffer and write the image (base+serial)
       * @return 0 if all are correct, -1 if we have and error
*/
int suGrabBase();

/**
       * Reset the internal count used in suSetBaseName
       * @param init_value the init value of the counter (int)
       * @see suSetBaseName 
       * @see suGrabBase
       * @return 0 if all are correct, -1 if we have and error
*/
int suSetBaseCounter(int init_value);

#endif

