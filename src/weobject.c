#include "weobject.h"

int  weQuadCreate(WEQuad *object,float *color,int texture_id) //Create a quad with normal pointing to outside ofthe screen
{
	float *ptr_vertex=NULL;

	if(object==NULL||color==NULL)
		return -1;

	object->texture_id=0;

	ptr_vertex=object->vertex;
	
	ptr_vertex[0]=-0.5;	ptr_vertex[1]=-0.5;	ptr_vertex[2]=0.0; ptr_vertex+=3;
	ptr_vertex[0]=0.5;	ptr_vertex[1]=-0.5;	ptr_vertex[2]=0.0; ptr_vertex+=3;
	ptr_vertex[0]=0.5;	ptr_vertex[1]=0.5;	ptr_vertex[2]=0.0; ptr_vertex+=3;
	ptr_vertex[0]=-0.5;	ptr_vertex[1]=0.5;	ptr_vertex[2]=0.0; ptr_vertex+=3;

	ptr_vertex=object->color;
	
	ptr_vertex[0]=color[0];	ptr_vertex[1]=color[1];	ptr_vertex[2]=color[2]; ptr_vertex+=3;
	ptr_vertex[0]=color[0];	ptr_vertex[1]=color[1];	ptr_vertex[2]=color[2]; ptr_vertex+=3;
	ptr_vertex[0]=color[0];	ptr_vertex[1]=color[1];	ptr_vertex[2]=color[2]; ptr_vertex+=3;
	ptr_vertex[0]=color[0];	ptr_vertex[1]=color[1];	ptr_vertex[2]=color[2]; ptr_vertex+=3;

	ptr_vertex=object->normal;
	
	ptr_vertex[0]=0.0f;	ptr_vertex[1]=0.0f;	ptr_vertex[2]=1.0f; ptr_vertex+=3;
	ptr_vertex[0]=0.0f;	ptr_vertex[1]=0.0f;	ptr_vertex[2]=1.0f; ptr_vertex+=3;
	ptr_vertex[0]=0.0f;	ptr_vertex[1]=0.0f;	ptr_vertex[2]=1.0f; ptr_vertex+=3;
	ptr_vertex[0]=0.0f;	ptr_vertex[1]=0.0f;	ptr_vertex[2]=1.0f; ptr_vertex+=3;

	ptr_vertex=object->texture;
	
	ptr_vertex[0]=0.0f;	ptr_vertex[1]=0.0f;	ptr_vertex+=2;
	ptr_vertex[0]=1.0f;	ptr_vertex[1]=0.0f;	ptr_vertex+=2;
	ptr_vertex[0]=1.0f;	ptr_vertex[1]=1.0f;	ptr_vertex+=2;
	ptr_vertex[0]=0.0f;	ptr_vertex[1]=1.0f;	ptr_vertex+=2;

	object->indices[0]=0;
	object->indices[1]=1;
	object->indices[2]=3;
	object->indices[3]=1;
	object->indices[4]=2;
	object->indices[5]=3;

	object->texture_id=texture_id;
	
	return 0;
}

int  weQuadCoordTex(float *ct,WEQuad *quad)
{
	float *ptr_vertex=NULL;

	if(ct==NULL|| quad==NULL);
		return 0;	

	ptr_vertex=quad->texture;
	
	ptr_vertex[0]=ct[0];	ptr_vertex[1]=ct[1];	ptr_vertex+=2;
	ptr_vertex[0]=ct[2];	ptr_vertex[1]=ct[3];	ptr_vertex+=2;
	ptr_vertex[0]=ct[4];	ptr_vertex[1]=ct[5];	ptr_vertex+=2;
	ptr_vertex[0]=ct[6];	ptr_vertex[1]=ct[7];	ptr_vertex+=2;

	return 0;
}

int  weVect3FromFile(char *filename,WEVect3 **ptrDest,int *n) //Read points from file, in n is the total points readed
{
	FILE *filePoints=NULL;
	int res,i;
	WEVect3 *ptrBuffer=NULL;

	if(filename==NULL || ptrDest==NULL || n==NULL)
		return -1;	

	filePoints= fopen(filename,"r");

	if(filePoints==NULL)
		return -2;

	fscanf(filePoints,"%i",n);

	ptrBuffer=ptrDest;

	ptrBuffer=(WEVect3*)malloc(sizeof(WEVect3)*(*n));

	if(ptrBuffer==NULL)
	{
		ptrBuffer=NULL;
		return -3;
	}

	for(i=0;i<*n;i++)
	{
	     res=fscanf(filePoints,"%f %f %f",&(ptrBuffer[i].x),&(ptrBuffer[i].y),&(ptrBuffer[i].z));

	     	if(res<3)
	     	{
			free(ptrBuffer);
			return -4;
		}
	
		//printf("%f, %f, %f\n",(ptrBuffer[i].x),(ptrBuffer[i].y),(ptrBuffer[i].z));
	}


	*ptrDest=ptrBuffer;
	fclose(filePoints);
	return 0;
}

int  weVect3Draw(WEVect3 *array,int nPoints, WEShaderVars *shader_vars, float *color, float *mv)
{
	float *colors;
	float *ptr_aux;
	int i;
	float *vertex;

	if(array==NULL || shader_vars==NULL || color==NULL || mv==NULL)
		return -1;

	colors=(float*)malloc(sizeof(float)*3*nPoints);
	ptr_aux=colors;

	for(i=0;i<nPoints;i++)
	{
		ptr_aux[0]=color[0];
		ptr_aux[1]=color[1];
		ptr_aux[2]=color[2];

		ptr_aux+=3;
	}

	glEnableVertexAttribArray(shader_vars->color_attrib);
	glEnableVertexAttribArray(shader_vars->vertex_attrib);

	glDisable(GL_TEXTURE_2D);

	glVertexAttribPointer(shader_vars->color_attrib,3,GL_FLOAT,0,0,colors);
	glVertexAttribPointer(shader_vars->vertex_attrib,3,GL_FLOAT,0,0,&(array->x));	

	if(mv!=NULL)
		glUniformMatrix4fv(shader_vars->modelview,1,0,mv);

	glDrawArrays(GL_POINTS,0,nPoints);

	glDisableVertexAttribArray(shader_vars->color_attrib);
	glDisableVertexAttribArray(shader_vars->vertex_attrib);

	free(colors);
	return 0;
}

int  wePointDraw(WEPoint *array,int nPoints,WEShaderVars *shader_vars, float *mv)
{
	float *ptr_aux;
	int i;
	float *vertex;

	if(array==NULL || shader_vars==NULL)
		return -1;


	glEnableVertexAttribArray(shader_vars->color_attrib);
	glEnableVertexAttribArray(shader_vars->vertex_attrib);

	glDisable(GL_TEXTURE_2D);

	glVertexAttribPointer(shader_vars->color_attrib,3,GL_FLOAT,0,sizeof(WEPoint),&(array->color));
	glVertexAttribPointer(shader_vars->vertex_attrib,3,GL_FLOAT,0,sizeof(WEPoint),&(array->vertex));	

	if(mv!=NULL)
		glUniformMatrix4fv(shader_vars->modelview,1,0,mv);

	glDrawArrays(GL_POINTS,0,nPoints);

	glDisableVertexAttribArray(shader_vars->color_attrib);
	glDisableVertexAttribArray(shader_vars->vertex_attrib);

	return 0;
}