#ifndef _WE_SPLASH_
#define _WE_SPLASH_

#include <stdio.h>
#include <string.h>
#include <SOIL/SOIL.h>
#include "glsut.h"
#include "we_opengl_hd.h"
#include "weobject.h"
#include "wemat.h"


int	weSplashInit(char *path_res, float *projection);
int	weSplashPlay();
int 	weSplashDeallocate();

#endif

