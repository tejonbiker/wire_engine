
#include <wemat.h>

/*
 The matrix must be supplied in Column Major Order
 |1 2 3|
 |4 5 6|
 =[1 4 2 5 3 6]
*/

void wematIdentity(float *m)
{	
	memset(m, 0, sizeof(float)*16);
	m[0]=1.0f;
	m[5]=1.0f;
	m[10]=1.0f;
	m[15]=1.0f;
	
}

void wematTranslateScale(float x,float y, float z, float s, float *m)
{
	memset(m,0,sizeof(float)*16);
	
	m[0]=s;
	m[5]=s;
	m[10]=s;
	m[15]=1.0f;
	
	m[12]=x;
	m[13]=y;
	m[14]=z;
}

void wematAddTranslate(float x,float y, float z, float *m)
{
	float buffer_m[16];
	float buffer_m2[16];

	wematIdentity(buffer_m);
		
	buffer_m[12]+=x;
	buffer_m[13]+=y;
	buffer_m[14]+=z;

	wematMult(m,buffer_m,buffer_m2);
	memcpy(m,buffer_m2,sizeof(float)*16);

}

int wematRotate(float polar,float azimutal, float *m)
{
	float rotPole[16],rotAzi[16];
	float rotResult[16]={0};
	float sinPolar,cosPolar;
	float sinAzi,cosAzi;
	
	wematIdentity(rotPole);
	wematIdentity(rotAzi);
	
	sinPolar=sin(polar);
	cosPolar=cos(polar);
	
	rotPole[5]=cosPolar;
	rotPole[6]=sinPolar;
	
	rotPole[9]=-sinPolar;
	rotPole[10]=cosPolar;
	
	sinAzi=sin(azimutal);
	cosAzi=cos(azimutal);
	
	rotAzi[0]=cosAzi;
	rotAzi[2]=-sinAzi;
	
	rotAzi[5]=1;
	rotAzi[8]=sinAzi;
	
	rotAzi[10]=cosAzi;
	
	wematMult(rotPole, rotAzi, rotResult);
	wematMult(m,rotResult,rotPole);
	
	memcpy(m,rotPole ,sizeof(float)*16);
	return 0;
}


void wematMult(float *A, float *B, float *C)
{
	int i,j,k;
	
	memset(C, 0, sizeof(float)*16);
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			for(k=0;k<4;k++)
			{
				C[i*4 +j]+=A[i*4+k]*B[k*4+j];
			}
		}
	}
}

int     wematMultVector(float *m, float *vec4, float *res4)
{
	int i,j,k;
	
	memset(res4, 0, sizeof(float)*4);
	for(i=0;i<4;i++)
	{
		for(j=0;j<1;j++)
		{
			for(k=0;k<4;k++)
			{
				res4[i+j]+=m[i*4+k]*vec4[k+j];
			}
		}
	}
}

void wematPrint(float *A)
{
	int i,j;
	
	printf("\n");
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			printf("%f ",A[j*4 +i]);
		}
		
		printf("\n");
	}
}

int wematODEtoGL(float *rot,float *pos,float *m16)
{
	//Espetial mention to http://www.ode.org/old_list_archives/2005-March/015281.html
	m16[0]=  rot[0];
	m16[1]=  rot[4];
	m16[2]=  rot[8];
	m16[3]=  0;
	m16[4]=  rot[1];
	m16[5]=  rot[5];
	m16[6]=  rot[9];
	m16[7]=  0;
	m16[8]=  rot[2];
	m16[9]=  rot[6];
	m16[10]= rot[10];
	m16[11]= 0;
	m16[12]= pos[0];
	m16[13]= pos[1];
	m16[14]= pos[2];
	m16[15]= 1;
	
	return 0;
}

int wematInverse(float *A, float *result)
{
	if(A==NULL || result==NULL)
	{
		return WEMAT_ERR_ARG;
	}
	
	float *a=NULL;
	int i,j,k;
	int RowNum;
	int ColNum;
	float divisionFactor;
	float multFactor;
	
	a = (float*)malloc(sizeof(float)*16);
	wematIdentity(result);
	
	RowNum = 4;
	ColNum = 4;
	
	memcpy(a, A, sizeof(float)*16);
	
	for(i=0;i<ColNum;i++)
	{
		divisionFactor=a[ColNum*i +i];
		
		for(j=0;j<ColNum;j++)
		{
			a[ColNum*i +j]/=divisionFactor;
			result[ColNum*i +j]/=divisionFactor;
		}
		
		for(j=0;j<RowNum;j++)
		{
			if(j!=i)
			{
				multFactor=a[ColNum*j+i];
				for(k=0;k<ColNum;k++)
				{
					a[ColNum*j+k] = a[ColNum*j+k] - a[ColNum*i +k]*multFactor;
					result[ColNum*j+k] = result[ColNum*j+k] - result[ColNum*i +k]*multFactor;
				}
				
			}
		}
		
	}
	
	free(a);
	return 0;
}

int wematTransponse(float *A, float *result)
{
	int i,j;
	
	if(A==NULL || result==NULL)
		return WEMAT_ERR_ARG;
	
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			result[j+4*i]=A[j*4+i];
	
	return 0;
}

void    wematAddScale(float sx,float sy,float sz, float *m)
{
	
	float buffer_m[16];
	float buffer_m2[16];

	if(m==NULL)
		return;

	/*	
	m[0]=m[0]*sx;
	m[5]=m[5]*sy;
	m[10]=m[10]*sz;
	*/

	wematIdentity(buffer_m);
		
	buffer_m[0]*=sx;
	buffer_m[5]*=sy;
	buffer_m[10]*=sz;

	wematMult(m,buffer_m,buffer_m2);
	memcpy(m,buffer_m2,sizeof(float)*16);
}

int		wematOrtho(float left, float right, float bottom, float top, float nearVal, float farVal, float *m)
{
	if(m==NULL)
		return -1;

	/*
	printf("left: %f\n",left);
	printf("right: %f\n",right);
	printf("bottom: %f\n",bottom);
	printf("top: %f\n",top);
	printf("nearVal: %f\n",nearVal);
	printf("farVal: %f\n",farVal);
	*/
	
	m[0]=2/(right-left);
	m[1]=m[2]=m[3]=m[4]=0;
	m[5]=2/(top-bottom);
	m[6]=m[7]=m[8]=m[9]=0;
	m[10]=-2/(farVal-nearVal);
	m[11]=0;
	m[12]=-(right+left)/(right-left);
	m[13]=-(top+bottom)/(top-bottom);
	m[14]=-(farVal + nearVal)/(farVal - nearVal);
	m[15]=1;
	
	//wematPrint(m);
	
	return 0;
}