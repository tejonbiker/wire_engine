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


//RPi headers!!!
#include <wiringPi.h> 
#include <wiringPiI2C.h>
#include <linux/i2c-dev.h>

#define RPI_SERVER

#define LED 0

void dostuff (int sock,int accel);

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
	//disable sample rate division
	wiringPiI2CWriteReg8(accel,0x19,0x00);


	if(argc<=1)
	{
		printf("No port especified\n");
		return -1;
	}

	//Socket Connection
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
        	error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		error("ERROR on binding");
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	
	while (1) {
		//printf("Esperando Conexion...\n");
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0) 
			error("ERROR on accept");
		pid = fork();
		if (pid < 0)
			error("ERROR on fork");
		if (pid == 0)  {
			close(sockfd);
			dostuff(newsockfd,accel);
			exit(0);
		}
		else close(newsockfd);
		printf("Cerrando Conexion\n");
	} /* end of while */
	close(sockfd);
}

void dostuff (int sock,int accel)
{
	int n,m;
	char buffer[256];
	int i;
	short accelerations[3];
	char buffer_in[64];
	char buffer_out[64];
	int low, high;
	int result;
	
	n = write(sock,"1989\0",5);

	printf("Total bytes writted: %i\n",n);
	
	while(n>0)
	{
		printf("Waiting input from client...\n");
		n = read(sock,buffer,255);

		buffer[n]='\0';
		printf("Character recieved: %s\n",buffer);

		if(buffer[0]=='A')
		{
			ioctl(accel,I2C_SLAVE,0x68);
			buffer_in[0]=0x3B;                
			result=write(accel,buffer_in,1);
			result=read(accel,buffer_in,14);

			for(i=0;i<3;i++)
			{
				low=buffer_in[i*2+1];
				high=buffer_in[i*2];
				accelerations[i]=(short)(((unsigned char)low) + ((unsigned char)(high))*256);
			}

			memset(buffer_out,0,64);
			sprintf(buffer_out,"%hi %hi %hi\0",accelerations[0],accelerations[1],accelerations[2]);
			printf("%s\n",buffer_out);
			write(sock,buffer_out,strlen(buffer_out));

		}else if(buffer[0]=='S')
		{
			printf("sudo halt");
			//system("sudo halt");
		}
		else
		{
			printf("Input value not recognized!!!\n");
		}
	}

}

