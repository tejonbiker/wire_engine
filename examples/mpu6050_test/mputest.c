/* A simple server in the internet domain using TCP
 The port number is passed as an argument */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

//RPi headers!!!
#include <wiringPi.h> 
#include <wiringPiI2C.h>

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
	int result;
	off_t real_offset;

	/*
	printf("Ajustando GPIO...\n");

	#ifdef RPI_SERVER
	//setup GPIO
	if(wiringPiSetup()==-1)
	{
		printf("Error al arrcancar WiringPi");
		return -2;
	}

	for(i=0;i<8;i++)
	{
		pinMode(i,OUTPUT);
		digitalWrite(i,0);
	}
	#endif

	printf("GPIO Listos\n");
	*/

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
	//Disable FSYNC and enable 1 KHz sample rate (enable DLPF)
	wiringPiI2CWriteReg8(accel,0x26,0x01);
	//disable sample rate division
	wiringPiI2CWriteReg8(accel,0x25,0x00);

	//Open the file log
	mpu_log_ptr=fopen("mpu6050.log","wt");

	if(mpu_log_ptr==NULL)
	{
		printf("Sorry, the file cannot be created, be sure you have permissions\n");
		return -1;
	}

	
	start_time=clock();
	i=0;
	while(1)
	{

		/*
		//Request Accel for X
		low=wiringPiI2CReadReg8(accel,0x3C);
                high=wiringPiI2CReadReg8(accel,0x3B);
                accelerations[0]=(short)(((unsigned char)low) + ((unsigned char)(high))*256);

		//Request Accel   for Y
		low=wiringPiI2CReadReg8(accel,0x3E);
                high=wiringPiI2CReadReg8(accel,0x3D);
                accelerations[1]=(short)(((unsigned char)low) + ((unsigned char)(high))*2568);

		//Request Accel  for Z
		low=wiringPiI2CReadReg8(accel,0x40);
		high=wiringPiI2CReadReg8(accel,0x3F);
		accelerations[2]=(short)(((unsigned char)low) + ((unsigned char)(high))*256);
		printf("%hi %hi %hi\n",accelerations[0],accelerations[1],accelerations[2]);
		//fprintf(mpu_log_ptr,"%hi %hi %hi\n",accelerations[0],accelerations[1],accelerations[2]);

		//fflush(mpu_log_ptr);
		sleep(1);
		*/
		
		buffer_read[0]=0x68 | 0x00;  //Address - Write
		buffer_read[1]=0x3B;           //Register - Start
		result=write(accel,buffer_read,2);

		buffer_read[0]=0x68 | 0x08;  //Address - Read
		result=write(accel,buffer_read,1);
		result=read(accel,buffer_read,14);
		
		
		for(i=0;i<14;i++)
		{
			printf("%x, ",buffer_read[i]);
		}
		printf("\n");
		

		/*		
		//Request Accel  for Z
		low=buffer_read[9];
		high=buffer_read[8];
		accelerations[2]=(short)(((unsigned char)low) + ((unsigned char)(high))*256);
		printf("%hi\n",accelerations[2]);
		*/
		
	
	

		//sleep(1);

		/*
		MPU_RAM_LOG[i*3]=accelerations[0];
		MPU_RAM_LOG[i*3+1]=accelerations[1];
		MPU_RAM_LOG[i*3+2]=accelerations[2];
		*/

		data_written++;
		i++;

		sample_print=500;
		if(data_written==sample_print)
		{
			end_time=clock();
			printf("%i sampled, time %lf\n", sample_print,(double)(end_time - start_time)/CLOCKS_PER_SEC);
			data_written=0;
			start_time=end_time;
		}
	}
}
