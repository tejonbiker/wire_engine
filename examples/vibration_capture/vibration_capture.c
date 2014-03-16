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

//RPi headers!!!
#include <wiringPi.h> 
#include <wiringPiI2C.h>
#include <linux/i2c-dev.h>

#define RPI_SERVER

#define LED 0

void error(const char *msg)
{
    perror(msg);
    exit(1);
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

	LIIR highpass;

	if(argc!=2)
	{
		printf("Usage: vibration_capture filter_matlab.fcf\n");	
		return;
	}

	address=0x68;
	accel=wiringPiI2CSetup(address);

	if(accel<0)
	{
		printf("Error to access Acceleromteter");
		return 0;
	}

	printf("Conected to %x, result connection: %i\n",address,accel);

	//Enable The power up of the accelerometer
	wiringPiI2CWriteReg8(accel,0x6B,0x00);
	//Enable 16 bit format
	wiringPiI2CWriteReg8(accel,0x21,0x01);
	//Disable FSYNC and enable 1 KHz sample rate (disable DLPF)
	wiringPiI2CWriteReg8(accel,0x1A,0x00);
	//Enable sample rate division to 29 Samples per second
	wiringPiI2CWriteReg8(accel,0x19,250);
	
	memset(ptr_buffer_before,0,14);


	result = LIIRMatlabRead(&highpass,argv[1]);

	if(result<0)
	{
		printf("An error ocurred during read file %s\n",argv[1]);
		return -1;
	}
	
	while(1)
	{

		ioctl(accel,I2C_SLAVE,0x68);
		ptr_buffer_current[0]=0x3B;                
		result=write(accel,ptr_buffer_current,1);
		result=read(accel,ptr_buffer_current,6);
		
		result=memcmp(ptr_buffer_before,ptr_buffer_current,14);

		if(result!=0)
		{	
			data_written++;
			for(i=0;i<3;i++) 
			{	
				low=buffer_read[i*2+1];
				high=buffer_read[i*2];
				accelerations[0]=(short)(((unsigned char)low) + ((unsigned char)(high))*256);
				accel_float[i]=accelerations[0]*2/32768.0f;

				//printf("%f ",accel_float[i]);
			}
			//printf("\n");
		}

		ptr_exchange_buffer=ptr_buffer_current;
		ptr_buffer_current=ptr_buffer_before;
		ptr_buffer_before=ptr_exchange_buffer; 

		total_read++;
		sample_print=25;
		if(data_written==sample_print)
		{
			printf("%i sampled, total read: %i\n", sample_print,total_read);
			fflush(mpu_log_ptr);
			data_written=0;
			total_read=0;
		}
		
	}
}
