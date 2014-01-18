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
WEShaderVars	*shader_vars=NULL;

int win_width, win_height;
float proj_matrix[16];

float tri_vertex[9]={0.0,0.0,0.0,  1.0,0.0,0.0,  1.0,1.0,0.0};
float tri_color[9] ={0.0,0.0,0.0,  1.0,0.0,0.0,  1.0,1.0,0.0};
float tri_normal[9]={1.0,0.0,0.0,  1.0,0.0,0.0,  1.0,0.0,0.0};

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
	float normalMatrix[16];
    	float   color_points[3]={0.0,0.0,0.0};
    	float   result_cat[3]={0.0,0.0,0.0};
    	static float   t=0.0;
    	WEVect3 vect_result;
	float   rot_mat[16];
	float   rot_buff[16];
  
    	transition+=0.005f;
    	t+=0.005;

    	if(transition>1.0f)
		transition=1.0f;

    	glClearColor(1.0,1.0,1.0,transition);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//wematIdentity(proj_matrix);
	//glUniformMatrix4fv(points_shader_vars.projection,1,0,proj_matrix);

	wematIdentity(rot_mat);
	//wematIdentity(rot_buff);
	//wematRotate(t,t,rot_buff);
	//wematIdentity(mv_points);
	//wematMult(mv_points,rot_buff,rot_mat);
	//wematAddScale(0.20,0.20,0.20,rot_mat);
	//wematAddTranslate(0.5,0.5,0.0,rot_mat);
 	weObjDraw(&model,&points_shader_vars,rot_mat);
	

    	glFlush();
    	glFinish();
    	glpiSwapBuffers();
}

void 	getShaderVars()
{
	int i;
	GLint *ptrVars=NULL;

	memset(&points_shader_vars,0,sizeof(WEShaderVars));

	points_shader_vars.projection	  =glGetUniformLocation(ShaderInfo.program,"Projection");
	points_shader_vars.modelview      =glGetUniformLocation(ShaderInfo.program,"Modelview");
	points_shader_vars.vertex_attrib  =glGetAttribLocation(ShaderInfo.program,"Position");
	points_shader_vars.color_attrib   =glGetAttribLocation(ShaderInfo.program,"Colors");	
	points_shader_vars.normal_attrib  =glGetAttribLocation(ShaderInfo.program,"Normal");
	points_shader_vars.normal_matrix  =glGetUniformLocation(ShaderInfo.program,"NormalMatrix");
	points_shader_vars.program_id     =ShaderInfo.program;
	
	printf("Printing Querying Vars...\n");
	weShaderVarsPrint(&points_shader_vars);

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

	//glUniformMatrix4fv(points_shader_vars.projection,1,0,proj_matrix);

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
	//weObjLoad(stdout,"obj/cube.obj",&model);
	weObjLoad(stdout,"obj/raspberry.obj",&model);
	printf("obj load success...\n");
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

	glBindFramebuffer(GL_FRAMEBUFFER,0);
	
	glClearColor(1.0f,1.0f,1.0f,0.0f); 	

	while(!finish){
		display();
	}	

	glpiEnd();
	return 0;
}
