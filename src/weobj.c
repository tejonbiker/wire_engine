#include "weobj.h"

int weObjLoad(FILE *outmsg,char *filename,WEObjModel *model)
{
	char buffer[OBJ_BUFFER_LENGHT];
	char stringIndex[128];
	int lenghtStringIndex=0;
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
	char *token=NULL;
	int m, line_counter=0;
	float *ptr_vertex=NULL,*ptr_normal=NULL;

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

		//printf("String: %s\n",buffer);

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

		//line_counter++;
		//printf("line readed: %i\n",line_counter);

	}while(read_result!=NULL);



	if(outmsg!=NULL)
		fprintf(outmsg,"Total readed data:\n vertex: %i\n normal: %i\n faces: %i \n ", vertex_counter,normal_counter,face_counter);

	vertex_array = (float*)malloc(sizeof(float)*3*vertex_counter);
	normal_array = (float*)malloc(sizeof(float)*3*normal_counter);

	model->nFaces=face_counter; 
	model->faces=(WEFace*)malloc(sizeof(WEFace)*face_counter);

	model->vertex=(float*)malloc(sizeof(float)*9*face_counter); //only triangular
	model->normal=(float*)malloc(sizeof(float)*9*face_counter);

	ptr_vertex=model->vertex;
	ptr_normal=model->normal;

	rewind(fileobj);

	line_counter=0;
	do
	{
		read_result=fgets(buffer,OBJ_BUFFER_LENGHT,fileobj); 


		if(read_result==NULL)
		{
			continue;
			printf("...\n");
			return 0;
		}

		//printf("String: %s\n",buffer);

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
					while(cut_read[0]==' ')
						cut_read++;

					for(m=0;m<3;m++)
					{
						
						lenghtStringIndex=0;
						while(cut_read[0]!='/'&&cut_read[0]!=' ')
						{
							stringIndex[lenghtStringIndex]=cut_read[0];
							lenghtStringIndex++;
							cut_read++;
							//printf("%s\n",cut_read);

						}
						cut_read++;
						stringIndex[lenghtStringIndex]=0;
						//printf("token: %s, %s",stringIndex,cut_read);

						if(lenghtStringIndex!=0)
						{
							sscanf(stringIndex,"%i",face_indices+m);
							face_indices[m]--;
						}
						else
							face_indices[m]=0;
					}

					//printf("  %i,%i,%i  \n",face_indices[0],face_indices[1],face_indices[2]);

					
					model->faces[face_index].point[j].vertex[0]=vertex_array[face_indices[0]*3]; 
					model->faces[face_index].point[j].vertex[1]=vertex_array[face_indices[0]*3+1]; 
					model->faces[face_index].point[j].vertex[2]=vertex_array[face_indices[0]*3+2];

					model->faces[face_index].point[j].normal[0]=normal_array[face_indices[2]*3]; 
					model->faces[face_index].point[j].normal[1]=normal_array[face_indices[2]*3+1];
					model->faces[face_index].point[j].normal[2]=normal_array[face_indices[2]*3+2];

					ptr_vertex[0]=vertex_array[face_indices[0]*3]; 
					ptr_vertex[1]=vertex_array[face_indices[0]*3+1]; 
					ptr_vertex[2]=vertex_array[face_indices[0]*3+2];

					ptr_vertex+=3;

					ptr_normal[0]=normal_array[face_indices[2]*3]; 
					ptr_normal[1]=normal_array[face_indices[2]*3+1];
					ptr_normal[2]=normal_array[face_indices[2]*3+2];

					ptr_normal+=3;
					
					//faces_indices[1] is not used, this is for texture coordinates
				}
				face_index++;
			break;

			default:
				if(outmsg!=NULL) fprintf(outmsg,"Some diffent appear: %s\n",cut_read);

		};

		//line_counter++;
		//printf("line readed: %i\n",line_counter);

	}while(read_result!=NULL);

	/*
	printf("printing data...\n");
	for(j=0;j<face_counter;j++)
	{
		

		printf("face %i\n:",j);
		printf("Vertex: ");

		for(k=0;k<3;k++)
		{
			ptr1 = model->faces[j].point[k].vertex;
			printf("  %f, %f, %f, ",ptr1[0],ptr1[1],ptr1[2]);
		}

		printf("\n Normal:");
		for(k=0;k<3;k++)
		{
			ptr1 = model->faces[j].point[k].normal;
			printf("  %f, %f, %f, ",ptr1[0],ptr1[1],ptr1[2]);
		}

		printf("\n");
	}
	*/
	

	free(vertex_array);
	free(normal_array);
	fclose(fileobj);

	return 0;
}

int weObjDraw(WEObjModel *model, WEShaderVars *shader_vars, float *mv)
{
	float normal_matrix[16];

	if(model==NULL || shader_vars==NULL || mv==NULL)
		return -1;

	wematNormalMatrix(mv,normal_matrix);

	glUseProgram(shader_vars->program_id);

	glEnableVertexAttribArray(shader_vars->vertex_attrib);
	glEnableVertexAttribArray(shader_vars->normal_attrib);

	//glVertexAttribPointer(shader_vars->vertex_attrib,3,GL_FLOAT,0,sizeof(WEFace),model->faces[0].point[0].vertex);
	//glVertexAttribPointer(shader_vars->normal_attrib,3,GL_FLOAT,0,sizeof(WEFace),model->faces[0].point[0].normal);

	glVertexAttribPointer(shader_vars->vertex_attrib,3,GL_FLOAT,0,0,model->vertex);
	glVertexAttribPointer(shader_vars->normal_attrib,3,GL_FLOAT,0,0,model->normal);

	glUniformMatrix4fv(shader_vars->modelview,1,0,mv);
	glUniformMatrix4fv(shader_vars->normal_matrix,1,0,normal_matrix);

	//glDrawArrays(GL_TRIANGLES,0,model->nFaces);
	glDrawArrays(GL_TRIANGLES,0,model->nFaces*3);

	glDisableVertexAttribArray(shader_vars->vertex_attrib);
	glDisableVertexAttribArray(shader_vars->normal_attrib);
		
	return 0;
}