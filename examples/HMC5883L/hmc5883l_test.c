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
#include <math.h>

//RPi headers!!!
#include <wiringPi.h> 
#include <wiringPiI2C.h>
#include <linux/i2c-dev.h>


//For Guanajuato, Mexico: 5* 53' 48'' E,  that means: 5.88 Degrees
//that means: 0.10262 rads, negative
#define MAGNETIC_DECLINATION -0.10262f

#define RPI_SERVER

#define LED 0

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	int pid;
	char buffer[256];
	int i;
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	int accel=0;
	int low,high;
	short accelerations[3];
	int address;
	FILE *mpu_log_ptr=NULL;
	int	data_written=0;
	short MPU_RAM_LOG[20000*3];
	int sample_print=300;
	clock_t start_time,end_time;
	unsigned char buffer_read[14];
	unsigned char buffer_before[14];
	unsigned char *ptr_buffer_current=buffer_read;
	unsigned char *ptr_buffer_before=buffer_before;
	unsigned char *ptr_exchange_buffer=NULL;
	int result;
	off_t real_offset;
	int total_read=0;
	float heading;

	address=0x1E;
	accel=wiringPiI2CSetup(address);

	if(accel<0)
	{
		printf("Error to access Acceleromteter");
		return 0;
	}

	printf("Conected to %x, result connection: %i\n",address,accel);

	//8-average, 15 Hz default, normal measurement
	wiringPiI2CWriteReg8(accel,0x00,0x70);
	//Gain=5
	wiringPiI2CWriteReg8(accel,0x01,0xA0);
	//Continuous Measurement
	wiringPiI2CWriteReg8(accel,0x02,0x00);
	//disable sample rate division
	wiringPiI2CWriteReg8(accel,0x19,0x00);

	//Open the file log
	mpu_log_ptr=fopen("mpu6050.log","wt");

	if(mpu_log_ptr==NULL)
	{
		printf("Sorry, the file cannot be created, be sure you have permissions\n");
		return -1;
	}
	
	memset(ptr_buffer_before,0,14);

	
	start_time=clock();
	i=0;
	while(1)
	{
		ioctl(accel,I2C_SLAVE,0x1E);
		ptr_buffer_current[0]=0x03;                
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
				accelerations[i]=(short)(((unsigned char)low) + ((unsigned char)(high))*256);
				printf("%hi ",accelerations[i]);
				
			}
			//fprintf(mpu_log_ptr,"\n");
			
			heading = atan2(accelerations[2],accelerations[0])+MAGNETIC_DECLINATION;

			printf(" \t  %f ",heading*180.0f/3.1416f);

			printf("\n");
		}

		ptr_exchange_buffer=ptr_buffer_current;
		ptr_buffer_current=ptr_buffer_before;
		ptr_buffer_before=ptr_exchange_buffer;

		total_read++; 
		sample_print=15;
		if(data_written==sample_print)
		{
			end_time=clock();
			printf("%i sampled, time %lf, total read: %i\n", sample_print,(double)(end_time - start_time)/CLOCKS_PER_SEC,total_read);
			fflush(mpu_log_ptr);
			data_written=0;
			total_read=0;
			start_time=end_time;
		}
		
	}
}
