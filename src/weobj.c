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

		cut_read=read_result;

		while(cut_read[0]!=' '||cut_read[0]!='\t')
			cut_read++;

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

		};

	}while(read_result!=NULL);

	printf("Total readed data:\n vertex: %i\n normal: %i\n faces: %i \n ", vertex_counter,normal_counter,face_counter);

	return 0;
}

int weObjDraw(WEObjModel *model, WEShaderVars *shader_vars, float *mv)
{
	return 0;
}