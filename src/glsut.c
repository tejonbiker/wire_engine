#include "glsut.h"

/* A simple lib to load shaders(vertex and fragment), linking and querying from compiled program
*/

int  glsutLoadShaders(FILE *messages, BaseShader *info_shader, char *fragment_file, char *vertex_file)
{
	FILE *FragmentShader=NULL;
	FILE *VertexShader=NULL;
	char srcFragment[GLPL_MAX_SHADER_SIZE];
	char srcVertex[GLPL_MAX_SHADER_SIZE];
	char *tempRead=NULL;
	char *returnRead=NULL;
	
	if(info_shader==NULL || fragment_file==NULL || vertex_file==NULL)
	{
		if(messages!=NULL)
			fprintf(messages,"Error in pass arguments, someone is null\n");
		
		return SUT_ERR_ARG;
	}
	
	FragmentShader=fopen(fragment_file,"r");
	
	if(FragmentShader==NULL)
	{
		if(messages!=NULL)
			fprintf(messages,"Error in open %sn",fragment_file);
		return SUT_ERR_FILE;
	}
	
	VertexShader=fopen(vertex_file, "r");
	
	if(VertexShader==NULL)
	{
		if(messages!=NULL)
			fprintf(messages,"Error in open %sn",vertex_file);
		
		fclose(FragmentShader);
		return SUT_ERR_FILE;
	}
	
	tempRead=srcFragment;
	while(!feof(FragmentShader))
	{
		returnRead=fgets(tempRead, 1024, FragmentShader);
		if(returnRead==NULL)
			break;
		tempRead+=strlen(returnRead);
		
	}
	
	tempRead=srcVertex;
	while(!feof(VertexShader))
	{
		returnRead=fgets(tempRead, 1024, VertexShader);
		if(returnRead==NULL)
			break;
		tempRead+=strlen(returnRead);
	}
	
	fclose(VertexShader);
	fclose(FragmentShader);
	return glsutLoadShadersChar(messages, info_shader, srcFragment, srcVertex);
}


//Similar to glplotLoadShader but can recibe from char
int glsutLoadShadersChar(FILE *messages,BaseShader *info_shader,const  char *fragment_src, const char *vertex_src)
{
	GLint vertexShader;
	GLint fragmentShader;
	GLchar message[1024];
	GLint linkSucess,programHandle,compileSucess;
	
	
	if(info_shader==NULL || fragment_src==NULL || vertex_src==NULL)
	{
		if(messages!=NULL)
			fprintf(messages,"Error in pass arguments, someone is null\n");
		
		return SUT_ERR_ARG;
	}
	
	vertexShader=glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertex_src, 0);
	glCompileShader(vertexShader);
	
	//Quering for sucessfull compilation
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileSucess);
	
	if(compileSucess==GL_FALSE) //No? Well, extract the reason
	{
		glGetShaderInfoLog(vertexShader, sizeof(message), 0, message);
		if(messages!=NULL)
			fprintf(messages,"Error al compilar vertex shader: %s\n", message);
		return -1;
	}
	
	//let's continue with fragment shader
	fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragment_src, 0);
	glCompileShader(fragmentShader);
	
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileSucess);
	
	if(compileSucess==GL_FALSE)
	{
		glGetShaderInfoLog(fragmentShader, sizeof(message), 0, message);
		if(messages!=NULL)
			fprintf(messages,"Error al compilar fragment shader: %s\n", message);
		return -1;
	}
	
	//We need link the fragment and vertex together in "program" id
	programHandle = glCreateProgram();
	glAttachShader(programHandle, vertexShader);
	glAttachShader(programHandle, fragmentShader);
	glLinkProgram(programHandle);
	
	//Queryng for sucessfull linking
	glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSucess);
	
	if(linkSucess==GL_FALSE)
	{
		glGetProgramInfoLog(programHandle, sizeof(message), 0, &message[0]);
		if(messages!=NULL)
			fprintf(messages,"Error al enlazar shader: %s\n", message);
		return -3;
	} 
	
	info_shader->program=programHandle;
	info_shader->fragment_shader=fragmentShader;
	info_shader->vertex_shader=vertexShader;
	
	return 0;
}

int	glsutCheckLocations(int *base_loc,int nvars)
{
	int i;

	if(base_loc==NULL)
		return -1;

	for(i=0;i<nvars;i++)
	{

		printf("%i:%i",i,base_loc[i]);
		if(base_loc[i]==-1)
			return -(i+1);
	}

	return 0;
}

/*
int glsutLinkUniform(BaseShader *info_shader,char *varName,GLint *uniform)
{
	*uniform = glGetUniformLocation(info_shader->program,varName);
	return *uniform;
}

int glsutLinkAttrib(BaseShader *info_shader,GLint *attrib)
{
	*attrib = glGetAttribLocation(info_shader->program,varName);
	return *attrib;
}
*/