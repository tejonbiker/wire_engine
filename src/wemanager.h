#ifndef _WEMANAGER_
#define _WEMANAGER_

/*
	Library created to help manage internally the resources implicated of
	the library and simplify some aspects like the Shaders, variables and their
	locations, aditionally help to manage the path of the resources
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "we_opengl_hd.h"

//Library to load the shaders from text files and include the structures
#include "glsut.h"

//Some defines to indicate which shaders will be load
#define WE_POINTLINE_SH 	1
#define WE_TEXTTRIANGE_SH 	2
#define WE_TEXTLIGHT_SH 	4

/*	Init the manager system
		path_rsc: path to the folder of the resources (textures and shaders)

	Return
		0 if all correct, -1 indicates error (in progress the translate)
*/
int WEManInit(char *path_rsc);		

/*
	Setup the shaders to load in the base of path set in WEManInit();
		int shaders_to_load: load several shaders (current three, in development!!!) example
				     WE_POINTLINE_SH | WE_TEXTTRIANGLE | WE_TEXTLIGHT
	Return
		0 if all is correct, <0 indicates error (in development the translate)		
*/
int WEManLoadShaders(int shaders_to_load);

/*
	Set the ShProgrvar specified by the user, be careful, only use if you
	know what you are doing!!!
*/
int WEManSetUserShProgVar(ShProgVar *shader_info);
int WEManDisableSetUserShProgVar();

#endif

