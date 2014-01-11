#include <stdio.h>
#include "glpi.h"  //Library that init the EGL context for OpenGL and OpeGL headers
#include "glsut.h" //Very simple routines to load text files and compile as shaders
#include "wemat.h" //Simple library to manage 4x4 matrix transformation for OpenGL

/*
Very Standalone example of OpenGL ES 2.0, uses EGL that draw directly in the video buffer of the RPi
Worflow of the program:

	-Main
	-ELG Init
	-OpenGL init
	-Shader Load, compile and querying
	-precalculus of sine function
		-calculus of sine
		-setup the modelview matrix
        -display loop
		-Send modelview matrix
		-Setup Vertex Pointers
		-Call OpenGL Draw
		-Swap double buffer
*/

//Flag that indicates the end of the render loop
int finish=0;
BaseShader ShaderInfo; //Holds the basic information of the shaders (program, vertex and fragment id)

//Hold the location of the variables of the shader
typedef struct tagVarShaders{
	GLint Modelview;
	GLint Position;
	GLint Colors;
}VarShaders;

VarShaders varShaders; //instance of the structure, we can't use the structure directly

#define N_POINTS 200 //Number of the points to render (Static memory)

//Structure that holds the info to render (color + position)
typedef struct tagPoint{
	float pos[3];
	float color[3];
}Point;

//Array of points
Point PointArray[N_POINTS];

float modelview[16];    //Hold the modelview matrix
float modelBuffer[16];  //Buffer for operands


//Prototypes of functions
int  setupEGL(void);
void setupGL(void);
int  setupShaders(void);
void calculateSine(void);
void display(void);


int main(void)
{
	int res=0;

	res =setupEGL();
	if(res<0)
	{
		printf("Error in EGL: %i",res);
		return 0;
	}

	setupGL();

	res=setupShaders();
	if(res<0)
	{
		printf("Error in Shader: %i\n",res);
		return 0;
	}

	calculateSine();

	glBindFramebuffer(GL_FRAMEBUFFER,0);
	
	//White background
	glClearColor(1.0f,1.0f,1.0f,1.0f); 	

	while(!finish){
		display();
	}	

	glpiEnd();
	return 0;
}

int setupEGL()
{
	return	glpiInit(); //This function creates the EGL context
}

void setupGL()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepthf(1.0f);
	glDepthMask(GL_TRUE);	
}

int setupShaders()
{
	int res;
	
	res=glsutLoadShaders(stdout,&ShaderInfo,"fragment_shader","vertex_shader"); 
	//load shaders from file, put the basic information in ShaderInfo, all error messages 
	//are printed via "stdout"
	
	if(res<0)
	{
		return -1;
	}

	//Start to querying variables from shaders
	glUseProgram(ShaderInfo.program); //Ensure that the compiled program is used
	
	//Querying for the vars, remember, we have two types of variables: uniform and attribute
	//Uniform: Data that not changes with every call to draw, example the modelview 
	//transformation of entire model
	varShaders.Modelview=glGetUniformLocation(ShaderInfo.program,"Modelview");

	//Attrib: This type of data changes with every vertex that send to render, example
	//different colors for each 3D vertex (and the same vertex)
	varShaders.Position=glGetAttribLocation(ShaderInfo.program,"Position");
	varShaders.Colors=glGetAttribLocation(ShaderInfo.program,"Colors");

	if(varShaders.Modelview==-1 || varShaders.Position==-1 || varShaders.Colors==-1)
	{
		printf("Error in querying shader variables\n");
		return -1;
	}

	return 0;
}

void calculateSine()
{
	Point *ptrPoint=NULL; //Some pointers to avoid overwriting the same variable
	float *ptrColor=NULL;
	float *ptrPos=NULL;
	int i;
	float omega=5; //Frequenci of the sine, try with other values 
	
	ptrPoint = PointArray;

	//Evaluate the sin in the range [0:1,-1:1]
	//The color is a red color that variates according to the
	//Y position of the sine passing for other colors
	for(i=0;i<N_POINTS;i++,ptrPoint++)
	{
		ptrPos=(ptrPoint->pos);
		ptrPos[0]=i/(float)N_POINTS;			//X
		ptrPos[1]=sin(2*3.1416*omega*ptrPos[0]);	//Y
		ptrPos[2]=0.0f;					//Z:Depth

		ptrColor=(ptrPoint->color);
		ptrColor[0]=ptrPos[1];			//R
		ptrColor[1]=0.5;			//G
		ptrColor[2]=0.5;			//B
	}

	//Prepare the modelview matrix to show the sine plot in the range of [-1:1,-1;1]
	wematIdentity(modelview);
	wematAddTranslate(-0.5f,0.0f,0.0f,modelview);
	wematAddScale(2.0f,1.0f,1.0f,modelview);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(ShaderInfo.program); 	//Ensure you use the correct program before sendind data to the
						//Shader variables	

	//Send the modelview matrix (remember uniform variable)
	glUniformMatrix4fv(varShaders.Modelview,1,0,modelview);	

	//To send the data of colors and vertex is neccesary set some data first 
	//(remember this are attribute variables), be careful, this can lead to
	//segementation fault.

	//First enable the attribute pointers
	glEnableVertexAttribArray(varShaders.Colors);
	glEnableVertexAttribArray(varShaders.Position);

	//Pass basic information of the pointer, the parameter are as follow:
	//Location, dimension of the data(3D),normalize?,byte separation between two vertex,base pointer to the data
	glVertexAttribPointer(varShaders.Colors,3,GL_FLOAT,0,sizeof(Point),&(PointArray[0].color));
	glVertexAttribPointer(varShaders.Position,3,GL_FLOAT,0,sizeof(Point),&(PointArray[0].pos));

	//The vertex are in sequence, this mean we can use glDrawArrays, other case, like building a quad
	//with only 4 vertex, you need specify in which order you are drawing the vertices.
	//Draw from 0 to N_POINTS elements
	glDrawArrays(GL_LINE_STRIP,0,N_POINTS);
	
	//For the last (and recommended for security), disable AttribVertex
	//if you don't disable the attrib pointer you can lead to a 
	//segmentation fault, this is due that OpenGL search in the last
	//memory sector specified.
	glDisableVertexAttribArray(varShaders.Colors);
	glDisableVertexAttribArray(varShaders.Position);

	
	glFlush();	  //Force to flush all render commands to the framebuffer
	glFinish();	
	glpiSwapBuffers();//Swap the double buffer (if you don't swap you don't see nothing)
}

