#include <stdio.h>
#include "glpi.h"

#define GLPI
#include "glplot.h"

Matrix *testsurf=NULL;
float t=0;
float phi=0;
float theta=0;
#define SIZE_GRID 100


void display()
{
	int m,k;
	float amplitude;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	
	 t+=0.02;
	 theta+=0.02;
	 phi+=0.02;
 	amplitude=exp(-t*t*0.01)*cos(t);
 		for(k=0;k<SIZE_GRID;k++)
 		{
 			for(m=0;m<SIZE_GRID;m++)
 			{
 				testsurf->data[ (k*SIZE_GRID +m)*testsurf->col+ 1 ]=k;
 				testsurf->data[ (k*SIZE_GRID +m)*testsurf->col + 0 ]=m;
 
 				if(m*m + k*k == 0 )
 					testsurf->data[ (k*SIZE_GRID +m)*testsurf->col+ 2 ]= 1;
 				else
 					testsurf->data[ (k*SIZE_GRID +m)*testsurf->col+ 2 ]= amplitude*SIZE_GRID*sin(sqrt((m-SIZE_GRID/2)*(m-SIZE_GRID/2) + (k-SIZE_GRID/2)*(k-SIZE_GRID/2)))/(sqrt((m-SIZE_GRID/2)*(m-SIZE_GRID/2) + (k-SIZE_GRID/2)*(k-SIZE_GRID/2)));
 			}
		}
	
	
	glplotLineStrip(testsurf);
	glplotApplyRotations(3.1416/6.0,3.1416/6.0);
	
	//printf("Dibujando\n");
	glFlush();
	glFinish();	
	glpiSwapBuffers();
}

void setup()
{
	testsurf=MatCreate(3,SIZE_GRID*SIZE_GRID);
}

int main(void)
{
	int res=0;

	res =glpiInit();

	printf("\nVerificando Resultado\n");

	if(res<0)
	{
		printf("Error: %i",res);
		return 0;
	}

	printf("\nPasando a Clear Color\n");

	glplotInit();
	setup();

	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glClearColor(1.0f,1.0f,1.0f,1.0f);

	while(1){
		display();

	}	

	glplotEnd();
	glpiEnd();
	return 0;
}
