#include <stdio.h>
#include "glpi.h"

/*
Simple example that shows a white_screen, the interesting part of this example resides in 
glpi.h and glpi.c, this files are that init the buffers in the raspberry pi, this way of work
don't use X window, draw direct in the screen.
*/

int finish=0;
int win_width, win_height;
float proj_matrix[16];

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
	glFlush();
	glFinish();	
	glpiSwapBuffers();
}

void setup()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepthf(1.0f);
	glDepthMask(GL_TRUE);
	//glDepthRange(0.0f, 1.0f);
	
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
