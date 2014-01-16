#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "wire_engine.h"
#include <SOIL/SOIL.h>

/*
Example that show how to use the OBJ loader, it's very simple for the moment.
*/

int finish=0;
CatmullArray tray_one;


typedef struct tagVarShaders{
	GLint	Projection;
	GLint	Modelview;
	GLint	Position;
	GLint	Colors;
}varShaders;

varShaders VarShaders;

//Estructuras para los shaders
BaseShader ShaderInfo;
WEShaderVars   points_shader_vars;

int win_width, win_height;
float proj_matrix[16];

WEObjModel model;

void display()
{
    static int i=0;
    static int counter_steps=0;
    static int current=0;
    static int next=1;
    int res;
    int MAX_STEPS=1024;
    float totalError;
    float MAX_ERROR=0.01f;  
    static float transition=0.0f;
    float   mv_points[16];
    float   color_points[3]={0.0,0.0,0.0};
    float   result_cat[3]={0.0,0.0,0.0};
    static float   t=0.0;
    WEVect3 vect_result;
  
    transition+=0.005f;
    t+=0.005;

    if(transition>1.0f)
	transition=1.0f;

    glClearColor(1.0,1.0,1.0,transition);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

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

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	printf("%i x %i \n",win_width,win_height);

	//Load obj object
	weObjLoad(stdout,"obj/cube.obj",&model);
}

int main(void)
{
	WEVect3 *ptrData=NULL;
	int nPoints;
	int res=0;
	int i;

	printf("Init EGL...\n");
	res =glpiInit();

	
	if(res<0)
	{
		printf("Error: %i\n",res);
		return 0;
	}

	
	setup();

	points_shader_vars.vertex_attrib= VarShaders.Position;
	points_shader_vars.color_attrib = VarShaders.Colors;
	points_shader_vars.modelview    =VarShaders.Modelview;

	glBindFramebuffer(GL_FRAMEBUFFER,0);
	
	glClearColor(1.0f,1.0f,1.0f,0.0f); 	

	while(!finish){
		display();
	}	

	glpiEnd();
	return 0;
}
