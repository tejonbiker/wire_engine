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

	printf("Conected to %x\n",address);

	//Enable The power up of the accelerometer
	wiringPiI2CWriteReg8(accel,0x6B,0x00);
	//Enable 16 bit format
	//wiringPiI2CWriteReg8(accel,0x21,0x01);

	i=0;
	while(1)
	{

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





		//Request Accel for X
		low=wiringPiI2CReadReg8(accel,0x3C);
                high=wiringPiI2CReadReg8(accel,0x3B);

                accelerations[0]=(short)(((unsigned char)low) + 256*((unsigned char)(high)));

                printf("%hi ",accelerations[0]);

		//Request Accel   for Y
		low=wiringPiI2CReadReg8(accel,0x3E);
                high=wiringPiI2CReadReg8(accel,0x3D);

                accelerations[1]=(short)(((unsigned char)low) + 256*((unsigned char)(high)));

                printf("%hi ",accelerations[1]);

		//Request Accel  for Z
		low=wiringPiI2CReadReg8(accel,0x40);
		high=wiringPiI2CReadReg8(accel,0x3F);

		accelerations[2]=(short)(((unsigned char)low) + 256*((unsigned char)(high)));

		printf("%hi \n",accelerations[2]);


		sleep(1);
	}
}
