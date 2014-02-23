/* A simple server in the internet domain using TCP
 The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

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

	

	i=0;
	while(1)
	{

		/*
		//Request Gyro for X
                low=wiringPiI2CReadReg8(accel,0x44);
                high=wiringPiI2CReadReg8(accel,0x43);

                accelerations[0]=(short)(((unsigned char)low) + 256*((unsigned char)(high)));

                printf("%hi ",accelerations[0]);

                //Request Gyro  for Y
                low=wiringPiI2CReadReg8(accel,0x46);
                high=wiringPiI2CReadReg8(accel,0x45);

                accelerations[1]=(short)(((unsigned char)low) + 256*((unsigned char)(high)));

                printf("%hi ",accelerations[1]);

                //Request Gyro for Z
                low=wiringPiI2CReadReg8(accel,0x48);
                high=wiringPiI2CReadReg8(accel,0x47);

                accelerations[2]=(short)(((unsigned char)low) + 256*((unsigned char)(high)));

                printf("%hi || ",accelerations[2]);
		*/


		//Request Accel for X
		low=wiringPiI2CReadReg8(accel,0x3C);
                high=wiringPiI2CReadReg8(accel,0x3B);
                accelerations[0]=(short)(((unsigned char)low) + 256*((unsigned char)(high)));
                //printf("%hi ",accelerations[0]);
		fprintf(mpu_log_ptr,"%hi ",accelerations[0]);


		//Request Accel   for Y
		low=wiringPiI2CReadReg8(accel,0x3E);
                high=wiringPiI2CReadReg8(accel,0x3D);
                accelerations[1]=(short)(((unsigned char)low) + 256*((unsigned char)(high)));
                //printf("%hi ",accelerations[1]);
		fprintf(mpu_log_ptr,"%hi ",accelerations[1]);

		//Request Accel  for Z
		low=wiringPiI2CReadReg8(accel,0x40);
		high=wiringPiI2CReadReg8(accel,0x3F);
		accelerations[2]=(short)(((unsigned char)low) + 256*((unsigned char)(high)));
		//printf("%hi \n",accelerations[2]);
		fprintf(mpu_log_ptr,"%hi\n",accelerations[2]);

		fflush(mpu_log_ptr);
		//sleep(1);

		data_written++;
		if(data_written==1000)
		{
			printf("1k sampled\n");
			data_written=0;
		}
	}
}
