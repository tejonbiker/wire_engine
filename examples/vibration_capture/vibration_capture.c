/*
	Code fixed by using 
	https://github.com/richardghirst/PiBits/tree/master/MPU6050-Pi-Demo
	as reference.
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#include "liir.h"
#include "wire_engine.h"

//RPi headers!!!
#include <wiringPi.h> 
#include <wiringPiI2C.h>
#include <linux/i2c-dev.h>

#define RPI_SERVER

#define LED 0

#define N_POINTS 1

BaseShader ShaderInfo; //Holds the basic information of the shaders (program, vertex and fragment id)

//Structure that holds the info to render (color + position)
typedef struct tagPoint{
	float pos[3];
	float color[3];
}Point;


//Hold the location of the variables of the shader
typedef struct tagVarShaders{
	GLint Modelview;
	GLint Position;
	GLint Colors;
}VarShaders;

VarShaders varShaders; //instance of the structure, we can't use the structure directly

Point PointArray[N_POINTS];

float modelview[16];    //Hold the modelview matrix
float modelBuffer[16];  //Buffer for operands


void display(); //OpenGL Draw Function

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int SetupAccelerometer(int *accel)
{
	int address=0x68;

	address=0x68;
	*accel=wiringPiI2CSetup(address);

	if(*accel<0)
	{
		printf("Error to access Acceleromteter");
		return -1;
	}

	//Enable The power up of the accelerometer
	wiringPiI2CWriteReg8(*accel,0x6B,0x00);
	//Enable 16 bit format
	wiringPiI2CWriteReg8(*accel,0x21,0x01);
	//Disable FSYNC and enable 1 KHz sample rate, enable DLPF to 44 Hz bandwidth
	wiringPiI2CWriteReg8(*accel,0x1A,0x02); //0x03
	//Enable sample rate division to 25 Samples per second
	wiringPiI2CWriteReg8(*accel,0x19,39); //250
}

int  InitShaders()
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

	wematIdentity(modelview);

	return 0;
}

int main(int argc, char *argv[])
{
	char buffer[256];
	int i;
	int n;
	int accel=0;
	int low,high;
	short accelerations[3];
	int address;
	FILE *mpu_log_ptr=NULL;
	int	data_written=0;
	int sample_print=300;
	unsigned char buffer_read[14];
	unsigned char buffer_before[14];
	unsigned char *ptr_buffer_current=buffer_read;
	unsigned char *ptr_buffer_before=buffer_before;
	unsigned char *ptr_exchange_buffer=NULL;
	int result;
	off_t real_offset;
	int total_read=0;
	float accel_float[3];
	double filter_accel;

	LIIR highpass;

	if(argc!=2)
	{
		printf("Usage: vibration_capture filter_matlab.fcf\n");	
		return;
	}

	//Adjust the accelerometer (see the function to know more details)
	result = SetupAccelerometer(&accel);

	if(result<0)
	{
		printf("Cannot connect to MPU6050\n");
	}
	
		
	//Load the coefficients of highpass filter
	result = LIIRMatlabRead(&highpass,argv[1]);

	if(result<0)
	{
		printf("An error has ocurred during reading the file %s\n",argv[1]);
		return -1;
	}
	
	//Clean the memory for avoid oversampling de MPU6050
	memset(ptr_buffer_before,0,14);


	//Setup EGL
	result=glpiInit();

	if(result<0)
	{
		printf("An error has ocurred during the init of EGL\n");
		return -1;
	}

	//Setup OpenGL 
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepthf(1.0f);
	glDepthMask(GL_TRUE);	

	result=InitShaders();
	
	if(result<0)
	{
		printf("An error has ocurred during the shader read\n");
		return -1;
	}

	
	while(1)
	{

		ioctl(accel,I2C_SLAVE,0x68);
		ptr_buffer_current[0]=0x3B;                
		result=write(accel,ptr_buffer_current,1);
		result=read(accel,ptr_buffer_current,6);
		result=memcmp(ptr_buffer_before,ptr_buffer_current,6);

		if(result!=0)
		{	
			data_written++;
			for(i=0;i<3;i++) 
			{	
				low=ptr_buffer_current[i*2+1];
				high=ptr_buffer_current[i*2];
				accelerations[0]=(short)(((unsigned char)low) + ((unsigned char)(high))*256);
				accel_float[i]=accelerations[0]*2/32768.0f;

				//printf("%f ",accel_float[i]);

			}
			//printf("\n");

			LIIRFilter2(&highpass,accel_float[0],&filter_accel);
			//printf("%f, %lf \t ",accel_float[0],filter_accel);
			//printf("\n");

			PointArray[0].pos[0]=filter_accel;
			PointArray[0].pos[1]=0;
			PointArray[0].pos[2]=0;

			PointArray[0].color[0]=128;
			PointArray[0].color[1]=100;
			PointArray[0].color[2]=65;

			
			display(); //Call The OpenGL Drawing
		}

		ptr_exchange_buffer=ptr_buffer_current;
		ptr_buffer_current=ptr_buffer_before;
		ptr_buffer_before=ptr_exchange_buffer; 

		total_read++;
		sample_print=25;
		if(data_written==sample_print)
		{
			printf("%i sampled, total read: %i\n",data_written,total_read);
			data_written=0;
			total_read=0;
		}
		
	}
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
	glDrawArrays(GL_POINTS,0,N_POINTS);
	
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
