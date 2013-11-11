#include <stdio.h>
#include "glpi.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "glsut.h"
#include <SOIL/SOIL.h>

/*
Simple example that shows a white_screen, the interesting part of this example resides in 
glpi.h and glpi.c, this files are that init the buffers in the raspberry pi, this way of work
don't use X window, draw direct in the screen.
*/

int finish=0;

typedef struct tagFiguraInfo{
    int nVertices;
    float *vertices;
    float *colors;
}FiguraInfo;

typedef struct tagErroContenedor{
    int longitud;
    float *ErrorAnterior;
    float *ErrorActual;
}ErrorContenedor;

typedef struct tagVarShaders{
	GLint	Projection;
	GLint	Modelview;
	GLint	Position;
	GLint	Colors;
}varShaders;

varShaders VarShaders;

int ErrorCreaDesde(ErrorContenedor *cont,FiguraInfo *fig);
int ErrorLimpia(ErrorContenedor *cont);
int FiguraCrea(FiguraInfo *fig, int nvertices);
int FiguraDibuja(FiguraInfo *fig);


int N=400;
float *arrCirculo=NULL;
float *arrColores=NULL;

float *errorSave;
ErrorContenedor ErrorEjes;

int nFiguras=9;
float **figuras;

FiguraInfo *Figuras=NULL;
FiguraInfo BufferFig;

//Estructuras para los shaders
BaseShader ShaderInfo;

int win_width, win_height;

void calculaCirculo()
{
    float t,paso;
    int i;
    float r=0.7f;
    int j=0;
    float *ptrFiguras=NULL;
    float *ptrColor=NULL;
    float *ptrColorFinal=NULL;
    float *ptrColorInicial=NULL;
    float a,b,k;

    float *coloresRand;

    if(arrCirculo!=NULL)
        free(arrCirculo);

    Figuras = (FiguraInfo*)malloc(sizeof(FiguraInfo)*nFiguras);

    for(i=0;i<nFiguras;i++)
        FiguraCrea(Figuras+i,N);

    FiguraCrea(&BufferFig,N);

    coloresRand=(float*)malloc(sizeof(float)*3*nFiguras*2);

    for(i=0;i<nFiguras*2*3;i++)
        coloresRand[i]=(rand()%1024)/1024.0f;

    paso=360.0f/(float)N;

    for(j=0,t=0.0f,i=0;j<N;t+=paso,i+=3,j++)
    {

        ptrFiguras=Figuras[0].vertices;
        ptrFiguras[i]=  r*cos(t*3.1416/180.0f);
        ptrFiguras[i+1]=r*sin(t*3.1416/180.0f);
        ptrFiguras[i+2]=0;

        ptrColor        =Figuras[0].colors;
        ptrColorInicial =coloresRand;
        ptrColorFinal    =ptrColorInicial+3;
        ptrColor[i]   = ptrColorInicial[0] + (ptrColorFinal[0]-ptrColorInicial[0])*j/(float)N;
        ptrColor[i+1] = ptrColorInicial[1] + (ptrColorFinal[1]-ptrColorInicial[1])*j/(float)N;
        ptrColor[i+2] = ptrColorInicial[2] + (ptrColorFinal[2]-ptrColorInicial[2])*j/(float)N;


        ptrFiguras=Figuras[1].vertices;
        ptrFiguras[i]=  r*cos(t*3.1416/180.0f) + 0.3*r*cos(11*t*3.1416/180.0f);
        ptrFiguras[i+1]=r*sin(t*3.1416/180.0f) + 0.3*r*sin(11*t*3.1416/180.0f);
        ptrFiguras[i+2]=0;

        ptrColor        =Figuras[1].colors;
        ptrColorInicial =coloresRand+6;
        ptrColorFinal   =ptrColorInicial+3;
        ptrColor[i]   = ptrColorInicial[0] + (ptrColorFinal[0]-ptrColorInicial[0])*j/(float)N;
        ptrColor[i+1] = ptrColorInicial[1] + (ptrColorFinal[1]-ptrColorInicial[1])*j/(float)N;
        ptrColor[i+2] = ptrColorInicial[2] + (ptrColorFinal[2]-ptrColorInicial[2])*j/(float)N;




        ptrFiguras=Figuras[2].vertices;
        ptrFiguras[i]= r*cos(t*3.1416/180.0f)*cos(20*t*3.1416/180.0f);
        ptrFiguras[i+1]=r*sin(t*3.1416/180.0f)*sin(20*t*3.1416/180.0f);
        ptrFiguras[i+2]=0;

        ptrColor        =Figuras[2].colors;
        ptrColorInicial =coloresRand+12;
        ptrColorFinal   =ptrColorInicial+3;
        ptrColor[i]   = ptrColorInicial[0] + (ptrColorFinal[0]-ptrColorInicial[0])*j/(float)N;
        ptrColor[i+1] = ptrColorInicial[1] + (ptrColorFinal[1]-ptrColorInicial[1])*j/(float)N;
        ptrColor[i+2] = ptrColorInicial[2] + (ptrColorFinal[2]-ptrColorInicial[2])*j/(float)N;





        ptrFiguras=Figuras[3].vertices;
        ptrFiguras[i]= r*cos(t*3.1416/180.0f)*exp(0.3*sin(20*t*3.1416/180.0f));
        ptrFiguras[i+1]=r*sin(t*3.1416/180.0f);
        ptrFiguras[i+2]=0;

        ptrColor        =Figuras[3].colors;
        ptrColorInicial =coloresRand+18;
        ptrColorFinal   =ptrColorInicial+3;
        ptrColor[i]   = ptrColorInicial[0] + (ptrColorFinal[0]-ptrColorInicial[0])*j/(float)N;
        ptrColor[i+1] = ptrColorInicial[1] + (ptrColorFinal[1]-ptrColorInicial[1])*j/(float)N;
        ptrColor[i+2] = ptrColorInicial[2] + (ptrColorFinal[2]-ptrColorInicial[2])*j/(float)N;




        ptrFiguras=Figuras[4].vertices;
        ptrFiguras[i]= r*cos(t*3.1416/180.0f);
        ptrFiguras[i+1]=r*sin(2*t*3.1416/180.0f);
        ptrFiguras[i+2]=0;

        ptrColor        =Figuras[4].colors;
        ptrColorInicial =coloresRand+24;
        ptrColorFinal   =ptrColorInicial+3;
        ptrColor[i]   = ptrColorInicial[0] + (ptrColorFinal[0]-ptrColorInicial[0])*j/(float)N;
        ptrColor[i+1] = ptrColorInicial[1] + (ptrColorFinal[1]-ptrColorInicial[1])*j/(float)N;
        ptrColor[i+2] = ptrColorInicial[2] + (ptrColorFinal[2]-ptrColorInicial[2])*j/(float)N;


	ptrFiguras=Figuras[5].vertices;
        ptrFiguras[i]=  2*(t/360.0f -0.5);
        ptrFiguras[i+1]=sin(20*t*3.1416/180.0f)*(0.5*exp( -50*(t/360.0f - 0.3)*(t/360.0f - 0.3) ) +
						 0.5*exp( -50*(t/360.0f - 0.8)*(t/360.0f - 0.8) ));
        ptrFiguras[i+2]=0;

        ptrColor        =Figuras[5].colors;
        ptrColorInicial =coloresRand+30;
        ptrColorFinal   =ptrColorInicial+3;
        ptrColor[i]   = ptrColorInicial[0] + (ptrColorFinal[0]-ptrColorInicial[0])*j/(float)N;
        ptrColor[i+1] = ptrColorInicial[1] + (ptrColorFinal[1]-ptrColorInicial[1])*j/(float)N;
        ptrColor[i+2] = ptrColorInicial[2] + (ptrColorFinal[2]-ptrColorInicial[2])*j/(float)N;




	ptrFiguras=Figuras[6].vertices;
        ptrFiguras[i]= 0.7*cos(10*t*3.1416/180.0f)*exp( -40*(t/360.0f - 0.5)*(t/360.0f - 0.5) );
        ptrFiguras[i+1]=2*(t/360.0f-0.5);
        ptrFiguras[i+2]=0;

        ptrColor        =Figuras[6].colors;
        ptrColorInicial =coloresRand+36;
        ptrColorFinal   =ptrColorInicial+3;
        ptrColor[i]   = ptrColorInicial[0] + (ptrColorFinal[0]-ptrColorInicial[0])*j/(float)N;
        ptrColor[i+1] = ptrColorInicial[1] + (ptrColorFinal[1]-ptrColorInicial[1])*j/(float)N;
        ptrColor[i+2] = ptrColorInicial[2] + (ptrColorFinal[2]-ptrColorInicial[2])*j/(float)N;



	//a/b
	a=0.625;
	b=0.5;
	k=4;
	ptrFiguras=Figuras[7].vertices;
        ptrFiguras[i]= (a-b)*cos(k*t*3.1416/180.0f) + b*cos(k*3.1416/180.0f*t*((a/b)-1));
        ptrFiguras[i+1]=(a-b)*sin(k*t*3.1416/180.0f)- b*sin(k*3.1416/180.0f*t*((a/b)-1));
        ptrFiguras[i+2]=0;

        ptrColor        =Figuras[7].colors;
        ptrColorInicial =coloresRand+42;
        ptrColorFinal   =ptrColorInicial+3;
        ptrColor[i]   = ptrColorInicial[0] + (ptrColorFinal[0]-ptrColorInicial[0])*j/(float)N;
        ptrColor[i+1] = ptrColorInicial[1] + (ptrColorFinal[1]-ptrColorInicial[1])*j/(float)N;
        ptrColor[i+2] = ptrColorInicial[2] + (ptrColorFinal[2]-ptrColorInicial[2])*j/(float)N;


	a=0.65*0.6;
	b=1*0.6;
	k=20;
	ptrFiguras=Figuras[8].vertices;
        ptrFiguras[i]= (a-b)*cos(k*t*3.1416/180.0f) + b*cos(k*3.1416/180.0f*t*((a/b)-1));
        ptrFiguras[i+1]=(a-b)*sin(k*t*3.1416/180.0f)- b*sin(k*3.1416/180.0f*t*((a/b)-1));
        ptrFiguras[i+2]=0;

        ptrColor        =Figuras[8].colors;
        ptrColorInicial =coloresRand+48;
        ptrColorFinal   =ptrColorInicial+3;
        ptrColor[i]   = ptrColorInicial[0] + (ptrColorFinal[0]-ptrColorInicial[0])*j/(float)N;
        ptrColor[i+1] = ptrColorInicial[1] + (ptrColorFinal[1]-ptrColorInicial[1])*j/(float)N;
        ptrColor[i+2] = ptrColorInicial[2] + (ptrColorFinal[2]-ptrColorInicial[2])*j/(float)N;

    }
}

int interpola(FiguraInfo *fig1, FiguraInfo *fig2, FiguraInfo *dest, float step)
{
    float *ptrVertices1=NULL,*ptrVertices2=NULL;
    float *ptrColor1=NULL, *ptrColor2=NULL;
    float *ptrColorDest=NULL, *ptrVerticesDest=NULL;
    int i;

    if(fig1==NULL || fig2==NULL || dest==NULL)
        return -1;


    for(i=0;i<dest->nVertices;i++)
    {
        ptrVertices1 =      fig1->vertices;
        ptrVertices2 =      fig2->vertices;
        ptrColor1    =      fig1->colors;
        ptrColor2    =      fig2->colors;
        ptrVerticesDest =   dest->vertices;
        ptrColorDest=       dest->colors;

        ptrColorDest[i*3]= ptrColor1[i*3] + (ptrColor2[i*3] - ptrColor1[i*3])*step;
        ptrColorDest[i*3+1]= ptrColor1[i*3+1] + (ptrColor2[i*3+1] - ptrColor1[i*3+1])*step;
        ptrColorDest[i*3+2]= ptrColor1[i*3+2] + (ptrColor2[i*3+2] - ptrColor1[i*3+2])*step;

        ptrVerticesDest[i*3]    = ptrVertices1[i*3]     + (ptrVertices2[i*3] - ptrVertices1[i*3])*step;
        ptrVerticesDest[i*3+1]  = ptrVertices1[i*3+1]   + (ptrVertices2[i*3+1] - ptrVertices1[i*3+1])*step;
        ptrVerticesDest[i*3+2]  = ptrVertices1[i*3+2]   + (ptrVertices2[i*3+2] - ptrVertices1[i*3+2])*step;
    }
}

int PID(FiguraInfo *fig1, FiguraInfo *fig2, FiguraInfo *dest,ErrorContenedor *cont, float *totalError)
{
    float P=0.05,I=0.0,D;

    float *ptrVertices1=NULL,*ptrVertices2=NULL;
    float *ptrColor1=NULL, *ptrColor2=NULL;
    float *ptrColorDest=NULL, *ptrVerticesDest=NULL;
    int i;
    float verticeError;
    float MAX_THRESHOLD=2.0f;
    float step;
    float *ptrErrorActual=NULL;
    float *ptrErrorAnterior=NULL;

    if(fig1==NULL || fig2==NULL || dest==NULL)
        return -1;

    *totalError=0;

    ptrErrorActual=cont->ErrorActual;
    ptrErrorAnterior=cont->ErrorAnterior;

    ptrVertices1 =      fig1->vertices;
    ptrVertices2 =      fig2->vertices;
    ptrColor1    =      fig1->colors;
    ptrColor2    =      fig2->colors;
    ptrVerticesDest =   dest->vertices;
    ptrColorDest=       dest->colors;

    for(i=0;i<dest->nVertices;i++)
    {
        
        ptrErrorActual[0]     =(ptrVertices2[0] - ptrVerticesDest[0]);
        ptrErrorActual[1]   =(ptrVertices2[1] - ptrVerticesDest[1]);
        ptrErrorActual[2]   =(ptrVertices2[2] - ptrVerticesDest[2]);

        *totalError+=fabs(ptrErrorActual[0]) +fabs(ptrErrorActual[1]) +fabs(ptrErrorActual[2]);

        ptrVerticesDest[0]    += (ptrErrorActual[0])*P    +  (ptrErrorActual[3]   - ptrErrorAnterior[0])*I ;
        ptrVerticesDest[1]  += (ptrErrorActual[1])*P  +  (ptrErrorActual[1] - ptrErrorAnterior[1])*I ;;
        ptrVerticesDest[2]  += (ptrErrorActual[2])*P  +  (ptrErrorActual[2] - ptrErrorAnterior[2])*I ;

        ptrErrorAnterior[0]   =ptrErrorActual[0];
        ptrErrorAnterior[1] =ptrErrorActual[1];
        ptrErrorAnterior[2] =ptrErrorActual[2];

        if(verticeError>MAX_THRESHOLD)
        {
            verticeError=MAX_THRESHOLD;
        }

        step = verticeError/MAX_THRESHOLD;

        ptrColorDest[0]= ptrColor1[0] + (ptrColor2[0] - ptrColor1[0])*step;
        ptrColorDest[1]= ptrColor1[1] + (ptrColor2[1] - ptrColor1[1])*step;
        ptrColorDest[2]= ptrColor1[2] + (ptrColor2[2] - ptrColor1[2])*step;

	ptrVertices1+=3;
    	ptrVertices2+=3;
    	ptrColor1+=3;
    	ptrColor2+=3;
    	ptrVerticesDest+=3;
    	ptrColorDest+=3;
    }
}

int FiguraCopia(FiguraInfo* dest, FiguraInfo *src)
{
    int i;
    if(dest==NULL || src==NULL)
        return -1;

    memcpy(dest->vertices,src->vertices,sizeof(float)*dest->nVertices*3);

    return 0;
}

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

    //interpola(Figuras+current, Figuras+next,&BufferFig,counter_steps/((float)(MAX_STEPS)));
    PID(Figuras+current,Figuras+next,&BufferFig,&ErrorEjes,&totalError);

    FiguraDibuja(&BufferFig);

    //SOIL_save_screenshot("pic1.bmp",SOIL_SAVE_TYPE_BMP,0,0,win_width,win_height);

    if(totalError<MAX_ERROR)
    {
        current++;
        current=current%nFiguras;
        next++;
        next=next%nFiguras;

        counter_steps=0;
        FiguraCopia(&BufferFig,Figuras+current);
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
	}*/
}

void setup()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepthf(1.0f);
	glDepthMask(GL_TRUE);
	//glDepthRange(0.0f, 1.0f);

	if(glsutLoadShaders(stdout,&ShaderInfo,"fragment_shader","vertex_shader")<0)
	{
		exit(0);
	}

	printf("Shaders loaded succesfully\n");

	getShaderVars();

	srand(time(NULL));
    	calculaCirculo();
    	FiguraCopia(&BufferFig,Figuras);

    	errorSave=(float*)malloc(sizeof(float)*N);
    	memset(errorSave,0,sizeof(float)*N);
    	ErrorCreaDesde(&ErrorEjes,&BufferFig);
    	ErrorLimpia(&ErrorEjes);	

	glpiGetWindowSize(&win_width,&win_height);

	printf("%i x %i \n",win_width,win_height);
	glLineWidth(8.0f);
}

int FiguraCrea(FiguraInfo *fig, int nvertices)
{
    if(fig==NULL|| nvertices<=0)
        return -1;

    fig->nVertices=nvertices;
    fig->vertices   =(float*)malloc(sizeof(float)*nvertices*3);

    if(fig->vertices==NULL)
        return -2;

    fig->colors     =(float*)malloc(sizeof(float)*nvertices*3);
    if(fig->colors==NULL)
    {
        free(fig->vertices);
        return -3;
    }

    return 0;
}

int FiguraDibuja(FiguraInfo *fig)
{
	glUseProgram(ShaderInfo.program);
	glEnableVertexAttribArray(VarShaders.Position);
	glEnableVertexAttribArray(VarShaders.Colors);

	glVertexAttribPointer(VarShaders.Position,3,GL_FLOAT,0,0,fig->vertices);
	glVertexAttribPointer(VarShaders.Colors,3,GL_FLOAT,0,0,fig->colors);

	glDrawArrays(GL_LINE_LOOP,0,fig->nVertices);
}

int ErrorCreaDesde(ErrorContenedor *cont,FiguraInfo *fig)
{
    if(cont==NULL || fig==NULL)
        return -1;

    cont->ErrorActual=(float*)malloc(sizeof(float)*3*fig->nVertices);
    cont->ErrorAnterior=(float*)malloc(sizeof(float)*3*fig->nVertices);

    cont->longitud=fig->nVertices;

    return 0;
}

int ErrorLimpia(ErrorContenedor *cont)
{
    if(cont==NULL)
        return -1;

    memset(cont->ErrorActual,0,sizeof(float)*3*cont->longitud);
    memset(cont->ErrorAnterior,0,sizeof(float)*3*cont->longitud);

    return 0;
}

int main(void)
{
	int res=0;

	res =glpiInit();

	if(res<0)
	{
		printf("Error: %i",res);
		return 0;
	}

	setup();

	glBindFramebuffer(GL_FRAMEBUFFER,0);
	
	//the only activity in this example is chagne the color of the screen
	glClearColor(1.0f,1.0f,1.0f,1.0f); 	

	while(!finish){
		display();
	}	

	glpiEnd();
	return 0;
}
