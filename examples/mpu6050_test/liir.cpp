/*
 *  tiir.c
 *  IIRFilter
 *
 *  Created by José Federico  Ramos Ortega on 12/11/12.
 *  Copyright 2012 Facultad De Ingeniería Electronica. All rights reserved.
 *
 */

#include "liir.h"


int	LIIRInit(LIIR *filter, double *b, double *a, int lenghtCoef, int lenghtInput)
{
	int i;
	
	if(filter==NULL || a==NULL || b==NULL || lenghtCoef<=0 || lenghtInput<=0)
		return -1;
	
	filter->lenghtInput=lenghtInput;
	filter->Co=lenghtCoef;
	filter->n=0;
	
	filter->input = (double*)malloc(sizeof(double)*lenghtInput);
	
	if(filter->input==NULL)
		return LIIR_ERR_MEM;
	
	filter->output = (double*)malloc(sizeof(double)*lenghtInput);
	
	if(filter->output==NULL)
	{
		free(filter->input);
		return LIIR_ERR_MEM;
	}
	
	filter->a = (double*)malloc(sizeof(double)*lenghtCoef);
	
	if(filter->a==NULL)
	{
		free(filter->output);
		free(filter->input);
		return LIIR_ERR_MEM;
	}
	
	filter->b = (double*)malloc(sizeof(double)*lenghtCoef);
	
	if(filter->b==NULL)
	{
		free(filter->b);
		free(filter->output);
		free(filter->input);
		return LIIR_ERR_MEM;
	}
	
	filter->v = (double*)malloc(sizeof(double)*lenghtInput);
	
	if(filter->v==NULL)
	{
		free(filter->b);
		free(filter->output);
		free(filter->input);
		free(filter->b);
		return LIIR_ERR_MEM;
	}
	
	
	for(i=0;i<lenghtCoef;i++)
	{
		filter->a[i]=a[i];
		filter->b[i]=b[i];
	}
	
	memset((void*)filter->output,0,sizeof(double)*lenghtInput);
	memset((void*)filter->input,0,sizeof(double)*lenghtInput);
	memset((void*)filter->v,0,sizeof(double)*lenghtInput);
	
	return 0;
}

int LIIRFree(LIIR *filter)
{
	if(filter==NULL)
		return 1;
	
	if(filter->a!=NULL)
		free(filter->a);
	
	if(filter->b!=NULL)
		free(filter->b);

	if(filter->output!=NULL)
		free(filter->output);
	
	if(filter->input!=NULL)
		free(filter->input);
	
	if(filter->v!=NULL)
		free(filter->v);
	
	memset((void*)filter, 0, sizeof(LIIR));


	return 0;
}

int LIIRFilter2f(LIIR *filter, double input, double *output)
{
	int i;
	int indexIn;
	int indexOut;
	int indexV;
	double tempOut=0;
	double *VInput;
	double *VOutput;
	double *VV;
	int n;

	if(filter==NULL || output ==NULL)
		return -1;
	
	if(filter->n == filter->lenghtInput)
		return LIIR_END_INPUT;
	
	n = filter->n;
	
	VInput = filter->input;
	VOutput = filter->output;
	VV = filter->v;
	
	VInput[n]=input;
	
	for(i=0;i<n;i++)
	{
	}
	
 	
	return 0;
}

int	LIIRFilter(LIIR *filter, double input, double *output)
{
	int i;
	int indexIn;
	int indexOut;
	double tempOut=0;
	int mod=0;
	
	if(filter==NULL || output ==NULL)
		return -1;
	
	if(filter->n == filter->lenghtInput)
		return LIIR_END_INPUT;
		
	filter->input[filter->n]=input;
	
	
	printf("n=%i: ",filter->n);
	
	for(i=0;i<filter->Co;i++)
	{
		indexIn  = filter->n + i-filter->Co+1;
		indexOut = indexIn-1;
		
		if(indexIn>=0)
		{
			tempOut+= filter->input[indexIn]*filter->b[filter->Co-i-1];
			printf(" X[%i]*B[%i]  ",indexIn,filter->Co-i-1);
			//printf(" X-(%lf)*(%lf) ",filter->input[indexIn],filter->b[filter->Co-i-1]);
			
			
		}
		
		if(indexOut>=0)
		{
			
			if(filter->n < filter->Co)
			{			
			   tempOut-= filter->output[indexOut]*filter->a[filter->Co-i];
			   printf(" Y[%i]*A[%i] ",indexOut,filter->Co-i);
			  // printf(" Y-(%lf)*(%lf) ",filter->output[indexOut],filter->a[filter->Co-i]);
			}
			else
			{
				if(mod==1)
				{
					tempOut-= filter->output[indexOut-1]*filter->a[filter->Co-i];
					printf(" Y[%i]*A[%i] ",indexOut,filter->Co-i);
					//printf(" Y-(%lf)*(%lf) ",filter->output[indexOut],filter->a[filter->Co-i]);
				}
				mod=1;
			}
			 
			
			/*
			tempOut-= filter->output[indexOut]*filter->a[filter->Co-i];
			printf(" Y[%i]*A[%i] ",indexOut,filter->Co-i);
			 */
			
		}
	}
	
	printf("\n");
	
	filter->output[filter->n]=tempOut;
	filter->n++;
	*output=tempOut;
	return 0;
}

int	LIIRFilter2(LIIR *filter, double input, double *output)
{
	int i;
	double tempOut=0;
	double *x;
	double *y;
	double *a;
	double *b;
	int n;
	
	if(filter==NULL || output ==NULL)
		return -1;
		
	y= filter->output;
	x = filter->input;
	a = filter->a;
	b = filter->b;
	n = filter->n;
	
	if(n>filter->Co)
	{
		for(i=0;i<filter->Co-1;i++)
			x[i]=x[i+1];
		x[i]=input;
	}
	else
	{
		x[n]=input;
	}
	
	if(n==0)
	{
		tempOut = x[0]*b[0];
		y[0]=tempOut;
		
	}
	else if(n <= filter->Co)
	{
		for(i=0;i<n+1;i++)
		{
			tempOut += b[n-i]*x[i];
		}
		
		for(i=1;i<n+1;i++)
		{
			tempOut -= a[n+1-i]*y[i-1];
		}
		
		y[n]=tempOut;
		
	}else
	{		
		for(i=0;i<filter->Co;i++)
		{
			tempOut += b[filter->Co-i-1]*x[i];
		}
		
		for(i=1;i<filter->Co;i++)
		{
			tempOut -= a[i]*y[filter->Co-i+1];
		}
		
	}
	
	tempOut = tempOut/a[0];
	
	if(n>filter->Co)
	{
		for(i=0;i<filter->Co;i++)
			y[i]=y[i+1];
		
		y[i]=tempOut;
	}
	
	filter->n++;
	*output=tempOut;
	return 0;
}


int LIIRRewind(LIIR *filter)
{
	if(filter==NULL)
		return -1;
	
	memset((void*)filter->output,0,sizeof(double)*filter->lenghtInput);
	memset((void*)filter->input,0,sizeof(double)*filter->lenghtInput);
	memset((void*)filter->v,0,sizeof(double)*filter->lenghtInput);
	
	filter->n=0;
	return 0;
}

int LIIRFileRead(LIIR *filter,char *filename)
{
	int nCoef,nInputs;
	int counter;
	double *a=NULL, *b=NULL;
	int i;
	
	FILE *file=NULL;
	
	if(filter==NULL || filename ==NULL)
		return -1;
	
	file = fopen(filename, "r");
	
	if(file==NULL)
		return LIIR_ERR_OPEN;
	
	counter = fscanf(file,"%i %i\n",&nCoef, &nInputs);
	
	if(counter != 2)
	{
		fclose(file);
		return LIIR_ERR_READ;
	}
	
	a = (double*)malloc(sizeof(double)*nCoef);
	
	if(a==NULL)
	{
		fclose(file);
		return LIIR_ERR_MEM;
	}
	
	b = (double*)malloc(sizeof(double)*nCoef);
	
	if(b==NULL)
	{
		free(a);
		fclose(file);
		return LIIR_ERR_MEM;
	}
	
	for(i=0;i<nCoef;i++)
	{
		counter = fscanf(file, "%lf",b+ i);
		
		if(counter==0)
		{
			free(a);
			free(b);
			fclose(file);
			return LIIR_ERR_READ;
		}
	}
	
	for(i=0;i<nCoef;i++)
	{
		counter = fscanf(file, "%lf",a + i);
		
		if(counter==0)
		{
			free(a);
			free(b);
			fclose(file);
			return LIIR_ERR_READ;
		}
	}
	
	counter = LIIRInit(filter, b, a, nCoef, nInputs);
	
	if(counter < 0)
	{
		free(a);
		free(b);
		fclose(file);
		return counter;
	}
	
	free(a);
	free(b);
	fclose(file);
	return 0;
}

int LIIRFileWrite(LIIR *filter, char *filename)
{
	FILE *file=NULL;
	int i;
	
	if(filter==NULL || filename ==NULL)
		return -1;
	
	file = fopen(filename, "w");
	
	if(file == NULL)
		return LIIR_ERR_OPEN;
	
	fprintf(file, "%i %i\n", filter->Co, filter->lenghtInput);
	
	for(i=0;i<filter->Co;i++)
	{
		fprintf(file,"%lf ",filter->b[i]);
	}
	
	fprintf(file, "\n");
	
	for(i=0;i<filter->Co;i++)
	{
		fprintf(file,"%lf ",filter->a[i]);
	}
	
	fclose(file);
	
	return 0;
}

//http://mechatronics.ece.usu.edu/yqchen/filter.c/MEMO.HTML

void filter(int ord, double *a, double *b, int np, double *x, double *y)
{
	
	int i,j;
	
	y[0]=b[0]*x[0];
	
	for (i=1;i<ord+1;i++)
		
	{
		
		y[i]=0.0;
		
		for (j=0;j<i+1;j++)
			
			y[i]=y[i]+b[j]*x[i-j];
		
		for (j=0;j<i;j++)
			
			y[i]=y[i]-a[j+1]*y[i-j-1];
		
	}
	
	/* end of initial part */
	
	for (i=ord+1;i<np+1;i++)
		
	{
		
		y[i]=0.0;
		
		for (j=0;j<ord+1;j++)
			
			y[i]=y[i]+b[j]*x[i-j];
		
		for (j=0;j<ord;j++)
			
			y[i]=y[i]-a[j+1]*y[i-j-1];
		
	}
	
} /* end of filter */


