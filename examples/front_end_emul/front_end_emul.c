#include <stdio.h>
#include "glpi.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "glsut.h"
#include "wemat.h"
#include "we_splash_screen.h"
#include <SOIL/SOIL.h>

/*
Simple example that shows a white_screen, the interesting part of this example resides in 
glpi.h and glpi.c, this files are that init the buffers in the raspberry pi, this way of work
don't use X window, draw direct in the screen.
*/

int finish=0;


typedef struct tagVarShaders{
	GLint	Projection;
	GLint	Modelview;
	GLint	Position;
	GLint	Colors;
}varShaders;

varShaders VarShaders;

//Estructuras para los shaders
BaseShader ShaderInfo;

int win_width, win_height;
float proj_matrix[16];

void display()
{
    static int i=0;
    static int counter_steps=0;
    static int current=0;
    static int next=1;
    int MAX_STEPS=1024;
    float totalError;
    float MAX_ERROR=0.01f;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    weSplashPlay();    

    glFlush();
    glFinish();
    glpiSwapBuffers();
}

void 	getShaderVars()
{
	int i;
	GLint *ptrVars=NULL;

	VarShaders.Projection=glGetUniformLocation(ShaderInfo.program,"Projection");
	VarShaders.Modelview=glGetUniformLocation(ShaderInfo.program,"Modelview");
	VarShaders.Position=glGetAttribLocation(ShaderInfo.program,"Position");
	VarShaders.Colors=glGetAttribLocation(ShaderInfo.program,"Colors");


	/*	
	ptrVars=(GLint*)(&VarShaders);
	for(i=0;i<sizeof(varShaders);i+=sizeof(int))
	{
		printf("%i\n",*ptrVars);
		if(*ptrVars<0)
		{
			printf("Error to get Vars from shader\n");
			exit(0);
		}
		ptrVars++;
	}
	*/
}

void window_resize()
{
	float aspect= win_width/(float)win_height;

	glViewport(0,0,win_width,win_height);

	if(aspect>=1.0f)
		wematOrtho(-1*aspect,1*aspect,-1,1,-1,1,proj_matrix);
	else
		wematOrtho(-1,1,-1/aspect,1/aspect,-1,1,proj_matrix);

	printf("Projection matrix result...\n");
	wematPrint(proj_matrix);

}

void setup()
{
	float testm[16];

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepthf(1.0f);
	glDepthMask(GL_TRUE);
	//glDepthRange(0.0f, 1.0f);

	printf("Loading shaders....\n");

	if(glsutLoadShaders(stdout,&ShaderInfo,"fragment_shader","vertex_shader")<0)
	{
		exit(0);
	}

	printf("Shaders loaded succesfully\n");

	getShaderVars();

	srand(time(NULL));

	glpiGetWindowSize(&win_width,&win_height);

	window_resize();
	weSplashInit(NULL,proj_matrix);

	

	printf("%i x %i \n",win_width,win_height);
}

int main(void)
{
	int res=0;

	printf("Init EGL...\n");
	res =glpiInit();

	if(res<0)
	{
		printf("Error: %i\n",res);
		return 0;
	}

	setup();

	glBindFramebuffer(GL_FRAMEBUFFER,0);
	
	glClearColor(1.0f,1.0f,1.0f,0.0f); 	

	while(!finish){
		display();
	}	

	glpiEnd();
	return 0;
}
