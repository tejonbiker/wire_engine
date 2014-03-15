#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "wire_engine.h"
#include <SOIL/SOIL.h>

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
BaseShader 	ShaderInfo;
WEShaderVars    points_shader_vars;

typedef struct tagBackgroud{
	GLint  texture;
	float  points[3*4];
	float  color[4*4];
	float  texcoord[2*4];
	GLuint indices[6];
}Background;

Background BackgroundImage;

int win_width, win_height;
float proj_matrix[16];

#define N_COLORPOINTS 200
#define N_SERPENTINES 5

int skip=3;

WEPoint arrayCPoints[N_COLORPOINTS];

typedef struct tagSerpentine{
	WEPoint 	arrayCPoints[N_COLORPOINTS];
	CatmullArray 	tray;
	float           colori[3];
	float		colorf[3];
}Serpentine;

Serpentine serpentines[N_SERPENTINES];

int fillVector=0;

void generatePoints(WEVect3 *array,int nPoints) 
{	
	int i;
	float *ptrV=NULL;

	if(array==NULL|| nPoints<=0)
		return;

	ptrV=(float*)array;
	for(i=0;i<nPoints;i++,ptrV+=3)
	{
		ptrV[0]= 2*( -(rand()%1024)/1024.0f + 0.5 );
		ptrV[1]= 2*( -(rand()%1024)/1024.0f + 0.5 );
		ptrV[2]=0.0f;
	}
}

void generateNextPoint(WEVect3 *array,int nPoints)
{
	int i;
	float *ptrV=NULL,*ptrVBef=NULL;

	ptrVBef=(float*)(array);

	for(i=1;i<nPoints-1;i++,ptrVBef+=3)
	{
		ptrV=ptrVBef+3;

		ptrVBef[0]=ptrV[0];
		ptrVBef[1]=ptrV[1];
		ptrVBef[2]=ptrV[2];
	}

	ptrV[0]= 0.90*2*( -(rand()%1024)/1024.0f + 0.5 );
	ptrV[1]= 0.90*2*( -(rand()%1024)/1024.0f + 0.5 );
	ptrV[2]=0.0f;
}

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
  
    	transition+=0.005f;
    	t+=0.005;

    	if(t>1.0f)
		t=0.0f;
    	
    	glClearColor(1.0,1.0,1.0,transition);
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if(transition>1.0f)
		transition=1.0f;
	else
	{
		glFlush();
    		glFinish();
    		glpiSwapBuffers();

		return;
	}		

   	res=weCatmullEvalArray(&tray_one,0.005,result_cat);
	
   	if(res==1)
   	{
		generateNextPoint(tray_one.points,tray_one.nPoints);
		weCatmullRewind(&tray_one);
   	}

    	wematIdentity(mv_points);
    	glUseProgram(ShaderInfo.program);
    	//weVect3Draw(tray_one.points,tray_one.nPoints,&points_shader_vars,color_points,mv_points);
    	//weVect3Draw((WEVect3*)result_cat,1,&points_shader_vars,color_points,mv_points);
	wePointDraw(arrayCPoints,fillVector,&points_shader_vars,mv_points);

    	if(fillVector<=N_COLORPOINTS-1)
    	{
	   	arrayCPoints[fillVector].vertex[0]=result_cat[0];
	    	arrayCPoints[fillVector].vertex[1]=result_cat[1];
		arrayCPoints[fillVector].vertex[2]=result_cat[2];
		
		fillVector++;
	}
	else
	{
		
		for(i=N_COLORPOINTS-1;i>=0;i--)
		{
			arrayCPoints[i].vertex[0]=arrayCPoints[i-1].vertex[0];
			arrayCPoints[i].vertex[1]=arrayCPoints[i-1].vertex[1];
			arrayCPoints[i].vertex[2]=arrayCPoints[i-1].vertex[2];
		}

		arrayCPoints[0].vertex[0]=result_cat[0];
		arrayCPoints[0].vertex[1]=result_cat[1];
		arrayCPoints[0].vertex[2]=result_cat[2];
		
	}

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

int setup(int argc, char **argv)
{
	int i;
	srand(time(NULL));

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

	for(i=0;i<N_COLORPOINTS;i++)
	{
		arrayCPoints[i].color[0]=1.0-(N_COLORPOINTS-i)/(float)N_COLORPOINTS;
		arrayCPoints[i].color[1]=1.0-(N_COLORPOINTS-i)/(float)N_COLORPOINTS;
		arrayCPoints[i].color[2]=1.0-(N_COLORPOINTS-i)/(float)N_COLORPOINTS;
	}


	printf("Loading texture: %s ... ",argv[1]);
	fflush(stdout);
	//Load background image
        BackgroundImage.texture=SOIL_load_OGL_texture(argv[1],SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,
				      SOIL_FLAG_MIPMAPS |SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	if(BackgroundImage.texture==0)
	{
			printf("Error in SOIL loading image %s, SOIL says: %s\n",argv[1],SOIL_last_result() );
			return -1;
	}

	printf("ok\n");
	printf("Textures loaded!!\n");

}

int main(int argc, int **argv)
{
	WEVect3 *ptrData=NULL;
	int nPoints;
	int res=0;
	int i;

	if(argc!=2)
	{
		printf("Usage: catmull_example.bin test_image.jpg\n");
		return -1;
	}

	printf("Init EGL...\n");
	res =glpiInit();

	
	if(res<0)
	{
		printf("Error: %i\n",res);
		return 0;
	}

	//res=weCatmullArrayFromFile("trayectories/test_points.txt",&tray_one);
	res=weCatmullArrayCreate(&tray_one,4);
	
	if(res<0)
	{
		printf("Error Cat: %i\n",res);
		return 0;
	}
	else
	{
		generatePoints(tray_one.points,tray_one.nPoints);

		printf("Total Points Readed Cat: %i\n",tray_one.nPoints);

		for(i=0;i<tray_one.nPoints;i++)
		{
			printf("%f,%f,%f\n",tray_one.points[i].x,tray_one.points[i].y,tray_one.points[i].z);
		}
	}

	
	res=setup(argc,argv);

	if(res<0)
		return -1;

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
