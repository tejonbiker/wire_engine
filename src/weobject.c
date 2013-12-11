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
	ptr_vertex[0]=0.0f;	ptr_vertex[1]=1.0f;	ptr_vertex+=2;
	ptr_vertex[0]=1.0f;	ptr_vertex[1]=1.0f;	ptr_vertex+=2;
	ptr_vertex[0]=1.0f;	ptr_vertex[1]=0.0f;	ptr_vertex+=2;

	object->indices[0]=0;
	object->indices[1]=1;
	object->indices[2]=3;
	object->indices[3]=1;
	object->indices[4]=2;
	object->indices[5]=3;

	object->texture_id=texture_id;
	
	return 0;
}

int  weQuadDelete(WEQuad *object)
{
	if(object==NULL)
		return -2;

	return 0;
}