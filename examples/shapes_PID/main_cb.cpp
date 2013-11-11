/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>


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

int ErrorCreaDesde(ErrorContenedor *cont,FiguraInfo *fig);
int ErrorLimpia(ErrorContenedor *cont);
int FiguraCrea(FiguraInfo *fig, int nvertices);

int N=400;
float *arrCirculo=NULL;
float *arrColores=NULL;

float *errorSave;
ErrorContenedor ErrorEjes;

int nFiguras=5;
float **figuras;

FiguraInfo *Figuras=NULL;
FiguraInfo BufferFig;

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

    float *coloresRand;

    if(arrCirculo!=NULL)
        free(arrCirculo);

    arrCirculo=(float*)malloc(sizeof(float)*N*3);
    arrColores=(float*)malloc(sizeof(float)*N*3);

    /*
    figuras=(float**)malloc(sizeof(float*)*nFiguras);

    for(i=0;i<nFiguras;i++)
        figuras[i]=(float*)malloc(sizeof(float)*N*3);
    */

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
    float P=0.01,I=0.0,D;

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

    for(i=0;i<dest->nVertices;i++)
    {
        ptrVertices1 =      fig1->vertices;
        ptrVertices2 =      fig2->vertices;
        ptrColor1    =      fig1->colors;
        ptrColor2    =      fig2->colors;
        ptrVerticesDest =   dest->vertices;
        ptrColorDest=       dest->colors;

        ptrErrorActual[i*3]     =(ptrVertices2[i*3] - ptrVerticesDest[i*3]);
        ptrErrorActual[i*3+1]   =(ptrVertices2[i*3+1] - ptrVerticesDest[i*3+1]);
        ptrErrorActual[i*3+2]   =(ptrVertices2[i*3+2] - ptrVerticesDest[i*3+2]);

        *totalError+=fabs(ptrErrorActual[i*3]) +fabs(ptrErrorActual[i*3+1]) +fabs(ptrErrorActual[i*3+2]);

        ptrVerticesDest[i*3]    += (ptrErrorActual[i*3])*P    +  (ptrErrorActual[i*3]   - ptrErrorAnterior[i*3])*I ;
        ptrVerticesDest[i*3+1]  += (ptrErrorActual[i*3+1])*P  +  (ptrErrorActual[i*3+1] - ptrErrorAnterior[i*3+1])*I ;;
        ptrVerticesDest[i*3+2]  += (ptrErrorActual[i*3+2])*P  +  (ptrErrorActual[i*3+2] - ptrErrorAnterior[i*3+2])*I ;

        ptrErrorAnterior[i*3]   =ptrErrorActual[i*3];
        ptrErrorAnterior[i*3+1] =ptrErrorActual[i*3+1];
        ptrErrorAnterior[i*3+2] =ptrErrorActual[i*3+2];

        if(verticeError>MAX_THRESHOLD)
        {
            verticeError=MAX_THRESHOLD;
        }

        step = verticeError/MAX_THRESHOLD;

        ptrColorDest[i*3]= ptrColor1[i*3] + (ptrColor2[i*3] - ptrColor1[i*3])*step;
        ptrColorDest[i*3+1]= ptrColor1[i*3+1] + (ptrColor2[i*3+1] - ptrColor1[i*3+1])*step;
        ptrColorDest[i*3+2]= ptrColor1[i*3+2] + (ptrColor2[i*3+2] - ptrColor1[i*3+2])*step;
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

static void display(void)
{
    static int i=0;
    static int counter_steps=0;
    static int current=0;
    static int next=1;
    int MAX_STEPS=1024;
    float totalError;
    float MAX_ERROR=0.01f;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(0.0,0.0,0.0);
    glLineWidth(4.0f);

    //counter_steps++;

    if(counter_steps==MAX_STEPS)
    {
        current++;
        current=current%nFiguras;
        next++;
        next=next%nFiguras;

        counter_steps=0;
        FiguraCopia(&BufferFig,Figuras+current);
    }

    //interpola(Figuras+current, Figuras+next,&BufferFig,counter_steps/((float)(MAX_STEPS)));
    PID(Figuras+current,Figuras+next,&BufferFig,&ErrorEjes,&totalError);

    if(totalError<MAX_ERROR)
    {
        current++;
        current=current%nFiguras;
        next++;
        next=next%nFiguras;

        counter_steps=0;
        FiguraCopia(&BufferFig,Figuras+current);
    }

    glBegin(GL_LINE_LOOP);
    for(i=0;i<N;i++)
    {
        //glColor3fv( Figuras[4].colors + i*3);
        //glVertex3fv(Figuras[4].vertices + i*3);
        glColor3fv(BufferFig.colors + i*3);
        glVertex3fv(BufferFig.vertices + i*3);
    }
    glEnd();

    glutSwapBuffers();
}

static void idle(void)
{
    glutPostRedisplay();
}

void init()
{
    /*
    float t,paso;
    int i;
    float r=0.5f;
    int j=0;

    arrCirculo=(float*)malloc(sizeof(float)*N*3);

    paso=360.0f/(float)N;

    for(j=0,t=0.0f,i=0;j<N;t+=paso,i+=3,j++)
    {
        arrCirculo[i]=r*cos(t*3.1416/180.0f);
        arrCirculo[i+1]=r*sin(t*3.1416/180.0f);
        arrCirculo[i+2]=0;
    }
    */

    srand(time(NULL));
    calculaCirculo();
    FiguraCopia(&BufferFig,Figuras);

    errorSave=(float*)malloc(sizeof(float)*N);
    memset(errorSave,0,sizeof(float)*N);
    ErrorCreaDesde(&ErrorEjes,&BufferFig);
    ErrorLimpia(&ErrorEjes);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Figuras");

    init();

    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glutMainLoop();

    return EXIT_SUCCESS;
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

