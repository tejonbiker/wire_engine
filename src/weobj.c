#include "weobj.h"

int weObjLoad(FILE *outmsg,char *filename,WEObjModel *model)
{
	char buffer[OBJ_BUFFER_LENGHT];
	FILE *fileobj=NULL;
	int face_counter=0,normal_counter=0,vertex_counter=0;
	char *read_result=NULL;
	char  *cut_read=NULL;
	float *vertex_array=NULL;
	float *normal_array=NULL;
	int  vertex_index=0, normal_index=0, face_index=0;
	int  face_indices[3]={0};
	float *ptr1,*ptr2;
	int j,k;

	if(filename==NULL || model==NULL)
	{
		return -1;

		if(outmsg!=NULL)
			fprintf(outmsg,"Some argument was NULL\n");

	}	

	fileobj=fopen(filename, "r");

	if(fileobj==NULL)
	{
		if(outmsg!=NULL)
			fprintf(outmsg,"Error to open file %s\n",filename);
		return -2;
	}

	
	do
	{
		read_result=fgets(buffer,OBJ_BUFFER_LENGHT,fileobj); 

		if(read_result==NULL)
			continue;

		printf("String: %s\n",buffer);

		cut_read=read_result;

		while(cut_read[0]==' '||cut_read[0]=='\t' || cut_read[0]==0)
		{
			cut_read++;
			printf("%s\n",cut_read);
		}

		if(cut_read[0]==0)		
			continue;

		switch(cut_read[0])
		{
			case '#':
				if(outmsg!=NULL)
					fprintf(outmsg,"Argument found: %s\n",cut_read);
			break;

			case 'v':
				if(cut_read[1]=='n')
				{
					normal_counter++;
				}else{
					vertex_counter++;
				}
			break;

			case 'f':
				face_counter++;
			break;

			default:
				if(outmsg!=NULL) fprintf(outmsg,"Some diffent appear: %s\n",cut_read);

		};

	}while(read_result!=NULL);


	fprintf(outmsg,"Total readed data:\n vertex: %i\n normal: %i\n faces: %i \n ", vertex_counter,normal_counter,face_counter);

	vertex_array = (float*)malloc(sizeof(float)*3*vertex_counter);
	normal_array = (float*)malloc(sizeof(float)*3*normal_counter);

	model->nFaces=face_counter;
	model->faces=(WEObjModel*)malloc(sizeof(WEObjModel)*face_counter);

	rewind(fileobj);

	do
	{
		read_result=fgets(buffer,OBJ_BUFFER_LENGHT,fileobj); 

		if(read_result==NULL)
			continue;

		printf("String: %s\n",buffer);

		cut_read=read_result;

		while(cut_read[0]==' '||cut_read[0]=='\t' || cut_read[0]==0)
		{
			cut_read++;
			printf("%s\n",cut_read);
		}

		if(cut_read[0]==0)		
			continue;

		switch(cut_read[0])
		{
			case '#':
				if(outmsg!=NULL)
					fprintf(outmsg,"Argument found: %s\n",cut_read);
			break;

			case 'v':
				if(cut_read[1]=='n')
				{

					cut_read+=2;
					sscanf(cut_read,"%f %f %f",normal_array + normal_index
						,normal_array + normal_index+1
						,normal_array + normal_index+2);
					normal_index+=3;
				}else{
					cut_read++;
					sscanf(cut_read,"%f %f %f",vertex_array + vertex_index
						,vertex_array + vertex_index+1
						,vertex_array + vertex_index+2);
					vertex_index+=3;	
				}
			break;

			case 'f':
				cut_read++;
				for(j=0;j<3; j++)
				{
					
					sscanf(cut_read,"%i/%i/%i",face_indices
							,face_indices+1
							,face_indices+2);

					/*
					face_indices[0]--;
					face_indices[1]--;
					face_indices[2]--;
					*/

					printf("  %i,%i,%i  ",face_indices[0],face_indices[1],face_indices[2]);

					model->faces[face_index].point[j].vertex[0]=vertex_array[face_indices[0]*3]; 
					model->faces[face_index].point[j].vertex[1]=vertex_array[face_indices[0]*3+1]; 
					model->faces[face_index].point[j].vertex[2]=vertex_array[face_indices[0]*3+2];

					model->faces[face_index].point[j].normal[0]=normal_array[face_indices[2]*3]; 
					model->faces[face_index].point[j].normal[1]=normal_array[face_indices[2]*3+1];
					model->faces[face_index].point[j].normal[2]=normal_array[face_indices[2]*3+2];

					//faces_indices[1] is not used, this is for texture coordinates
				}
				face_index++;
			break;

			default:
				if(outmsg!=NULL) fprintf(outmsg,"Some diffent appear: %s\n",cut_read);

		};
	}while(read_result!=NULL);

	printf("printing data...\n");

	for(j=0;j<face_counter;j++)
	{
		

		printf("face %i\n:",j);
		printf("Vertex: ");

		for(k=0;k<3;k++)
		{
			ptr1 = model->faces[j].point[k].vertex;
			printf("  %f, %f, %f",ptr1[0],ptr1[1],ptr1[2]);
		}

		printf("\n Normal:");
		for(k=0;k<3;k++)
		{
			ptr1 = model->faces[j].point[k].normal;
			printf("  %f, %f, %f",ptr1[0],ptr1[1],ptr1[2]);
		}

	}


	return 0;
}

int weObjDraw(WEObjModel *model, WEShaderVars *shader_vars, float *mv)
{
	return 0;
}