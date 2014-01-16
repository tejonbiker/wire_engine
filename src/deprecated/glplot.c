
/*
 *  glplot.c
 *  HCM
 *
 *  Created by José Federico  Ramos Ortega on 17/02/13.
 *  Copyright 2013 Facultad De Ingeniería Electronica. All rights reserved.
 *
 http://antonholmquist.com/blog/opengl-es-2-0-shader-lighting-examples/
 */

#include "glplot.h"

#ifdef __cplusplus
extern "C"
#endif

PlotStats		DrawingState[1];
PlotPatternData	PatternData[1];
float			rotMatrix[16];
float			scatterColor[3]={0};
int				AutoSet=1;

loc_Material			loc_material;
loc_DirectionalLight	loc_directionalLight;
u_DirectionalLightLoc   u_directionalLight;



/*Example of sinc ondulating
 /*
 t+=0.02;
 amplitude=exp(-t*t*0.01)*cos(t);
 for(k=0;k<SIZE_GRID;k++)
 {
 for(m=0;m<SIZE_GRID;m++)
 {
 testsurf->data[ (k*SIZE_GRID +m)*testsurf->col+ 1 ]=k;
 testsurf->data[ (k*SIZE_GRID +m)*testsurf->col + 0 ]=m;
 
 if(m*m + k*k == 0 )
 testsurf->data[ (k*SIZE_GRID +m)*testsurf->col+ 2 ]= 1;
 else
 testsurf->data[ (k*SIZE_GRID +m)*testsurf->col+ 2 ]= amplitude*SIZE_GRID*sin(sqrt((m-SIZE_GRID/2)*(m-SIZE_GRID/2) + (k-SIZE_GRID/2)*(k-SIZE_GRID/2)))/(sqrt((m-SIZE_GRID/2)*(m-SIZE_GRID/2) + (k-SIZE_GRID/2)*(k-SIZE_GRID/2)));
 }
 */


//http://antonholmquist.com/blog/opengl-es-2-0-shader-lighting-examples/

/*
const char *v_shader={
"uniform   mat4 Projection;\n"
"uniform   mat4 Modelview;\n"
"uniform   mat4 NormalMat;\n"
"attribute vec4 Position;\n"
"attribute vec4 Colors;\n"
"varying   vec4 DestColor;\n"
"attribute vec2 inTextureCoords;\n"
"attribute vec3 inNormals;\n"
"varying   vec2 fTextureCoords;\n"
"varying   vec3 ecNormal;\n"
"uniform   float t;\n"
"void main(void)\n"
"{\n"
"  gl_Position=Projection*Modelview*Position;\n"
"  fTextureCoords=inTextureCoords;"
"  DestColor=Colors;"
"  ecNormal=vec3(NormalMat*vec4(inNormals,0.0));\n"
"  ecNormal=ecNormal/length(ecNormal);\n"
"}\n"
};

const char *f_shader={

"struct DirectionalLight{\n"
"vec3 direction;\n"
"vec3 halfplane;\n"
"vec4 ambientColor;\n"
"vec4 diffuseColor;\n"
"vec4 specularColor;\n"
"};\n"
"struct Material{\n"
"	vec4 ambientFactor;\n"
"	vec4 diffuseFactor;\n"
"	vec4 specularFactor;\n"
"	float shininess;\n"
"};\n"
"uniform DirectionalLight u_directionalLight;\n"
"uniform Material u_material;\n"
"varying  vec4			DestColor;\n"
"varying  vec3          ecNormal;\n"
"varying  vec2			fTextureCoords;\n"
"uniform  sampler2D		Texture;\n"
"uniform  sampler2D		NextTexture;\n"
"uniform  int			plotPoints;\n"
"		   vec4			colorShape;\n"
"uniform  float			factorBlend;\n"
"\n"
"void main(void)\n"
"{\n"
"   if(plotPoints!=1)\n"
"	{\n"
"		float ecNormalDotLightDirection = max(0.0,dot(ecNormal,u_directionalLight.direction));\n"
"		float ecNormalDotLightHalfplane = max(0.0,dot(ecNormal,u_directionalLight.halfplane));\n"
"\n"
"		vec4 ambientLight=u_directionalLight.ambientColor*u_material.ambientFactor;\n"
"		vec4 diffuseLight=ecNormalDotLightDirection*u_directionalLight.diffuseColor*u_material.diffuseFactor;\n"
"\n"
"		vec4 specularLight = vec4(0.0,0.0,0.0,0.0);\n"
"\n"
"		if(ecNormalDotLightHalfplane>0.0)\n"
"		{\n"
"			specularLight=pow(ecNormalDotLightHalfplane,u_material.shininess)*u_directionalLight.specularColor * u_material.specularFactor;\n"
"		}\n"
"\n"
"		vec4 light = ambientLight + diffuseLight + specularLight;\n"
"\n"
"		colorShape= texture2D(Texture,fTextureCoords);\n"
"		gl_FragColor=vec4(fTextureCoords.x,fTextureCoords.y,0.0,1.0)/2.0 + colorShape/2.0 ;\n"	
"		gl_FragColor=light*colorShape;\n"
"	}\n"
"	else\n"
"		gl_FragColor=DestColor;\n"
"}\n"
};
*/

const char *v_shader={
"uniform   mat4 Projection;\n"
"uniform   mat4 Modelview;\n"
"uniform   mat4 NormalMat;\n"
"attribute vec4 Position;\n"
"attribute vec4 Colors;\n"
"varying   vec4 DestColor;\n"
"attribute vec2 inTextureCoords;\n"
"attribute vec3 inNormals;\n"
"varying   vec2 fTextureCoords;\n"
"varying   vec3 ecNormal;\n"
"uniform   float t;\n"
"vec4 light_pos;\n"
"void main(void)\n"
"{\n"
"  light_pos=vec4(0.0,0.0,0.0,0.0);"
"  gl_Position=Projection*Modelview*Position;\n"
"  fTextureCoords=inTextureCoords;"
"  DestColor=Colors;"
"}\n"
};

const char *f_shader={

"struct DirectionalLight{\n"
"vec3 direction;\n"
"vec3 halfplane;\n"
"vec4 ambientColor;\n"
"vec4 diffuseColor;\n"
"vec4 specularColor;\n"
"};\n"
"struct Material{\n"
"       vec4 ambientFactor;\n"
"       vec4 diffuseFactor;\n"
"       vec4 specularFactor;\n"
"       float shininess;\n"
"};\n"
"uniform DirectionalLight u_directionalLight;\n"
"uniform Material u_material;\n"
"varying  vec4                  DestColor;\n"
"varying  vec3          ecNormal;\n"
"varying  vec2                  fTextureCoords;\n"
"uniform  sampler2D             Texture;\n"
"uniform  sampler2D             NextTexture;\n"
"uniform  int                   plotPoints;\n"
"                  vec4                 colorShape;\n"
"uniform  float                 factorBlend;\n"
"\n"
"void main(void)\n"
"{\n"
"      gl_FragColor=DestColor;\n"
"}\n"
};


const char *ftext_shader={
"varying  vec4 DestColor;\n"
"void main(void)\n"
"{\n"
"  gl_FragColor=DestColor;\n"
"}\n"
};


/*
 The matrix must be supplied in Column Major Order
 |1 2 3|
 |4 5 6|
 =[1 4 2 5 3 6]
*/

void matrixIdentity(float *m)
{	
	memset(m, 0, sizeof(float)*16);
	m[0]=1.0f;
	m[5]=1.0f;
	m[10]=1.0f;
	m[15]=1.0f;
	
}

void matrixTranslateScale(float x,float y, float z, float s, float *m)
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

void matrixAddTranslate(float x,float y, float z, float *m)
{
	m[12]+=x;
	m[13]+=y;
	m[14]+=z;
	
}

void matrixMult(float *A, float *B, float *C)
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

int     matrixMultVector(float *m, float *vec4, float *res4)
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

void matrixPrint(float *A)
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

int matrixODEtoGL(float *rot,float *pos,float *m16)
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

int matrixInverse(float *A, float *result)
{
	if(A==NULL || result==NULL)
	{
		return MAT_ERR_ARG;
	}
	
	float *a=NULL;
	int i,j,k;
	int RowNum;
	int ColNum;
	float divisionFactor;
	float multFactor;
	
	a = (float*)malloc(sizeof(float)*16);
	matrixIdentity(result);
	
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

int matrixTransponse(float *A, float *result)
{
	int i,j;
	
	if(A==NULL || result==NULL)
		return MAT_ERR_ARG;
	
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			result[j+4*i]=A[j*4+i];
	
	return 0;
}

void    matrixAddScale(float sx,float sy,float sz, float *m)
{
	if(m==NULL)
		return;
	
	m[0]=m[0]*sx;
	m[5]=m[5]*sy;
	m[10]=m[10]*sz;
}

int		matrixOrtho(float left, float right, float bottom, float top, float nearVal, float farVal, float *m)
{
	if(m==NULL)
		return -1;
	
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
	
	//matrixPrint(m);
	
	return 0;
}

int glplotInit()
{
	//first step is the shader creation
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint programHandle;
	GLint  linkSucess;
	GLint compileSucess;
	GLchar message[256];
	int i,j;
	GLint *ptrLoc=NULL;
	int samples;
	
	//The source code of the shaders is in the begining of the file
	//Init with the shader creation, start with vertex shader
	vertexShader=glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &v_shader, 0);
	glCompileShader(vertexShader);

	//Quering for sucessfull compilation
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileSucess);
	
	if(compileSucess==GL_FALSE) //No? Well, extract the reason
	{
		glGetShaderInfoLog(vertexShader, sizeof(message), 0, message);
		printf("Error al compilar vertex shader: %s\n", message);
		return -1;
	}
	
	//let's continue with fragment shader
	fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &f_shader, 0);
	glCompileShader(fragmentShader);
	
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileSucess);
	
	if(compileSucess==GL_FALSE)
	{
		glGetShaderInfoLog(fragmentShader, sizeof(message), 0, message);
		printf("Error al compilar fragment shader: %s\n", message);
		return -2;
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
		printf("Error al enlzar los shaders: %s\n",message);
		return -3;
	}
	
	DrawingState[0].fragmentShader=fragmentShader;
	DrawingState[0].vertexShader=vertexShader;
	
	
	//Use the program, we need get some info from shaders.
	glUseProgram(programHandle);
	
	DrawingState[0].programHandle=programHandle;
	
	//Get the ids of the following vars in the shaders
	DrawingState[0].idModelview =		glGetUniformLocation(programHandle,"Modelview");
	DrawingState[0].idProjection=		glGetUniformLocation(programHandle,"Projection");
	DrawingState[0].idAttribPosition=	glGetAttribLocation(programHandle, "Position");
	DrawingState[0].idAttribColor=		glGetAttribLocation(programHandle, "Colors");
	DrawingState[0].idAttribTexture=    glGetAttribLocation(programHandle, "inTextureCoords");
	DrawingState[0].idAttribNormal=     glGetAttribLocation(programHandle, "inNormals");
	DrawingState[0].idTexture =			glGetUniformLocation(programHandle, "Texture");
	DrawingState[0].idplotPoints= PatternData[0].plotPoints =			glGetUniformLocation(programHandle, "plotPoints");
	

	if(DrawingState[0].idModelview==-1 || DrawingState[0].idProjection==-1 || DrawingState[0].idAttribPosition==-1 ||
           DrawingState[0].idAttribColor==-1 || DrawingState[0].idAttribTexture==-1 || DrawingState[0].idTexture==-1 ||
	PatternData[0].plotPoints==-1  || DrawingState[0].idAttribNormal<0)
		{
			printf("Error al enlazar las variables del shader\n");
			return 0;
		}
	
	//let's start querying for light parameters :S
	loc_material.ambientFactor = glGetUniformLocation(programHandle, "u_material.ambientFactor");
	loc_material.diffuseFactor = glGetUniformLocation(programHandle, "u_material.diffuseFactor");
	loc_material.shininess		=glGetUniformLocation(programHandle, "u_material.shininess");
	loc_material.specularFactor =glGetUniformLocation(programHandle, "u_material.specularFactor");
	
	loc_directionalLight.ambientColor = glGetUniformLocation(programHandle, "u_directionalLight.ambientColor"); 
	loc_directionalLight.diffuseColor = glGetUniformLocation(programHandle, "u_directionalLight.diffuseColor");
	loc_directionalLight.direction    = glGetUniformLocation(programHandle, "u_directionalLight.direction");
	loc_directionalLight.halfplane    = glGetUniformLocation(programHandle, "u_directionalLight.halfplane");
	loc_directionalLight.specularColor= glGetUniformLocation(programHandle, "u_directionalLight.specularColor");
	
	DrawingState[0].idNormalMat = glGetUniformLocation(programHandle, "NormalMat");
	
	if( loc_material.ambientFactor <= 0  || loc_material.diffuseFactor <= 0  || loc_material.shininess <= 0 || loc_material.specularFactor <= 0 ||
		loc_directionalLight.ambientColor <= 0 || loc_directionalLight.diffuseColor <= 0 || loc_directionalLight.direction <= 0 ||
		loc_directionalLight.halfplane <=0 || loc_directionalLight.specularColor <=0 || DrawingState[0].idNormalMat <= 0 )
	{
		printf("Error al enlazar las variables del shader\n");
		return 0;
	}

	
	//We need setup this vars to setup the window behavior
	matrixIdentity(DrawingState[0].Modelview);
	matrixIdentity(DrawingState[0].Projection);
	matrixIdentity(rotMatrix);
	matrixTranslateScale(0.5, 0.5, -0.3, 0.5, DrawingState[0].Modelview);
	
	//Upload the matrices and enable AttribArray, similar to GL_VERTEX_ARRAY but this need
	//be linked to a Attrib var in the shader, see function glplotQuickTest for a quick and dirty example
	glUniformMatrix4fv(DrawingState[0].idModelview, 1,0, DrawingState[0].Modelview);
	glUniformMatrix4fv(DrawingState[0].idProjection, 1,0, DrawingState[0].Projection);
	glEnableVertexAttribArray(DrawingState[0].idAttribPosition);
	glEnableVertexAttribArray(DrawingState[0].idAttribColor);
	
	//Now we proceed to build the textures for centroids
	
	printf("Indices de Textura: ");
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(N_CTEXT, (unsigned int*)PatternData[0].centroidText);
	for(i=0;i<N_CTEXT;i++)
	{
		glBindTexture(GL_TEXTURE_2D, PatternData[0].centroidText[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 8, 8, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,arraycTextures[i]);
		printf("%i,Error: %i\n",  PatternData[0].centroidText[i],glGetError());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	
	
	glGenTextures(1, (GLuint*)&(PatternData[0].whiteTexture));
	glBindTexture(GL_TEXTURE_2D, PatternData[0].whiteTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, 1, 8, 8, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,centext_0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	
	glEnable(GL_TEXTURE_2D);
	
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &samples);
	
	printf("Las unidades de Textura son: %i\n",samples);
	
	//Now establish a default values for light
	u_directionalLight.ambientColor[0]=0.1;
	u_directionalLight.ambientColor[1]=0.1;
	u_directionalLight.ambientColor[2]=0.1;
	u_directionalLight.ambientColor[3]=1.0;
	
	u_directionalLight.diffuseColor[0]=0.9;
	u_directionalLight.diffuseColor[1]=0.9;
	u_directionalLight.diffuseColor[2]=0.9;
	u_directionalLight.diffuseColor[3]=1.0;
	
	u_directionalLight.direction[0]=0.0;
	u_directionalLight.direction[1]=0.0;
	u_directionalLight.direction[2]=-1.0;
	
	
	u_directionalLight.halfplane[0]=0.0f;
	u_directionalLight.halfplane[1]=0.0f;
	u_directionalLight.halfplane[2]=1.0f;

	u_directionalLight.specularColor[0]=0.5;
	u_directionalLight.specularColor[1]=0.5;
	u_directionalLight.specularColor[2]=0.5;
	u_directionalLight.specularColor[3]=1.0;
	
	return 0;
}

int  glplotLoadShaders(FILE *messages, PlotShaderInfo *info_shader, char *fragment_file, char *vertex_file)
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
		
		return PLOT_ERR_ARG;
	}
	
	FragmentShader=fopen(fragment_file,"r");
	
	if(FragmentShader==NULL)
	{
		if(messages!=NULL)
			fprintf(messages,"Error in open %sn",fragment_file);
		return PLOT_ERR_FILE;
	}
	
	VertexShader=fopen(vertex_file, "r");
	
	if(VertexShader==NULL)
	{
		if(messages!=NULL)
			fprintf(messages,"Error in open %sn",vertex_file);
		
		fclose(FragmentShader);
		return PLOT_ERR_FILE;
	}
	
	tempRead=srcFragment;
	while(!feof(FragmentShader))
	{
		returnRead=fgets(tempRead, 1024, FragmentShader);
		tempRead+=strlen(returnRead);
	}
	
	tempRead=srcVertex;
	while(!feof(VertexShader))
	{
		returnRead=fgets(tempRead, 1024, VertexShader);
		tempRead+=strlen(returnRead);
	}
	
	fclose(VertexShader);
	fclose(FragmentShader);
	return glplotLoadShadersChar(messages, info_shader, srcFragment, srcVertex);
}

int glplotLoadShadersChar(FILE *messages,PlotShaderInfo *info_shader,const  char *fragment_src, const char *vertex_src)//Similar to glplotLoadShader but can recibe from char
{
	GLint vertexShader;
	GLint fragmentShader;
	GLchar message[1024];
	GLint linkSucess,programHandle,compileSucess;
	
	
	if(info_shader==NULL || fragment_src==NULL || vertex_src==NULL)
	{
		if(messages!=NULL)
			fprintf(messages,"Error in pass arguments, someone is null\n");
		
		return PLOT_ERR_ARG;
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

int glplotEnd()
{
	int i;
	
	glDeleteShader(DrawingState[0].vertexShader);
	glDeleteShader(DrawingState[0].fragmentShader);
	glDeleteProgram(DrawingState[0].programHandle);
	
	glDeleteTextures(N_CTEXT,(unsigned int*)PatternData[0].centroidText);
	
	glDeleteTextures(1,(unsigned int*)(&PatternData[0].whiteTexture));
	
	memset(PatternData + 0, 0, sizeof(PlotPatternData));
	memset(DrawingState + 0, 0, sizeof(PlotStats));
	
	return 0;
}

void glplotQuickTest()
{
	float points[]={0.0,0.0,  0.3,0.3};
	float colors[]={0.0,0.0,0.0, 1.0,0.0,0.0};
	GLuint indices[]={0,1};
	float model[16];
	
	glPointSize(3);
	
	glVertexAttribPointer(DrawingState[0].idAttribPosition, 2, GL_FLOAT, 0, 0, points);
	glVertexAttribPointer(DrawingState[0].idAttribColor, 3, GL_FLOAT, 0, 0, colors);
	
	matrixTranslateScale(0.5, 0.5, 0.0, 1, model);
	glUniformMatrix4fv(DrawingState[0].idModelview, 1, 0, model);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, indices);
	
	
	matrixIdentity(model);
	glUniformMatrix4fv(DrawingState[0].idModelview,1, 0, model);
	glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, indices);
	
}

int glplotHCM2D(Matrix *Samples, Matrix *U, Matrix *Centroids)
{
	
	int i,j;
	float rangeX[2];
	float rangeY[2];
	float translateCentroid[2];
	float scaleX,scaleY;
	float scale;
	float modelview[16];
	GLuint *indices=NULL;
	float *colors=NULL;
	float *vertices=NULL;
	float *centroidVertices=NULL;
	float *centroidTextures=NULL;
	float *centroidColor=NULL;
	int   loopClass;
	int   loopSample;
	float auxCentroid[2];
	float auxTextCoords[2]={1.0,1.0};
	float framecomp=0;
	
	
	if(Samples==NULL || U==NULL || Centroids==NULL)
		return -1;
	
	if(Samples->col!=2)
		return -2;
	

	indices= (GLuint*)malloc(sizeof(GLuint)*Samples->row);
	colors = (float*)malloc(sizeof(float)*Samples->row*3);
	vertices = (float*)malloc(sizeof(float)*Samples->row*2);
	
	memset(colors,0,sizeof(float)*Samples->row*3);
	
	rangeX[1]=rangeX[0]=Samples->data[0];
	rangeY[1]=rangeY[0]=Samples->data[1];
	
	
	for(i=0,j=0;j<Samples->row;i+=2,j++)
	{
		if(Samples->data[i] <rangeX[0] )
		{
			rangeX[0]=Samples->data[i];
		}
		
		if(Samples->data[i] >rangeX[1])
		{
			rangeX[1]=Samples->data[i];
		}
		
		if(Samples->data[i+1] <rangeY[0] )
		{
			rangeY[0]=Samples->data[i+1];
		}
		
		if(Samples->data[i+1]>rangeY[1])
		{
			rangeY[1]=Samples->data[i+1];
		}
		
		indices[j]=j;
		vertices[i]=(float)(Samples->data[i]);
		vertices[i+1]=(float)(Samples->data[i+1]);
	}
	
	translateCentroid[0]=(rangeX[1]-rangeX[0])/2.0 + rangeX[0];
	translateCentroid[1]=(rangeY[1]-rangeY[0])/2.0 + rangeY[0];
	
	
	if(rangeX[1]-rangeX[0] > rangeY[1]-rangeY[0])
		scale = 1.85/(rangeX[1]-rangeX[0]);
	else
		scale = 1.85/(rangeY[1]-rangeY[0]);
	 
	/*
	scaleX = 1.85/(rangeX[1]-rangeX[0]);
	scaleY = 1.85/(rangeY[1]-rangeY[0]);
	*/
	
	glPointSize(6);
	
	//Need fix this, the multiplication cannot be here, for simplicity
	matrixTranslateScale(-translateCentroid[0]*scale,-translateCentroid[1]*scale,0, scale, modelview);
	glUniformMatrix4fv(DrawingState[0].idModelview, 1, 0, modelview);
	
	for(loopSample=0;loopSample<U->col;loopSample++)
	{
		for(loopClass=0;loopClass<U->row;loopClass++)
		{
			if(U->data[loopSample + loopClass*U->col]==1)
			{
				colors[loopSample*3]  =class_colors[loopClass][0];
				colors[loopSample*3+1]=class_colors[loopClass][1];
				colors[loopSample*3+2]=class_colors[loopClass][2];
				break;
			}
		}
	}
	
	glEnableVertexAttribArray(DrawingState[0].idAttribPosition);
	glEnableVertexAttribArray(DrawingState[0].idAttribColor);
	
	glDisableVertexAttribArray(DrawingState[0].idAttribTexture);
	glDisable(GL_TEXTURE_2D);
	
	glUniform1i(PatternData[0].plotPoints, 1); //Tells to the shader we now plot points, to avoid the texture part
	glVertexAttribPointer(DrawingState[0].idAttribPosition, 2, GL_FLOAT, 0, 0, vertices);
	glVertexAttribPointer(DrawingState[0].idAttribColor, 3, GL_FLOAT, 0, 0, colors);
	
	glDrawElements(GL_POINTS, Samples->row, GL_UNSIGNED_INT, indices);
	
	//Now proceed to draw the centroids
	centroidVertices=(float*)malloc(  sizeof(float)*U->row*4*2);
	centroidTextures=(float*)malloc( sizeof(float)*U->row*4*2);
	
	glUniform1i(PatternData[0].plotPoints, 0);
	glEnableVertexAttribArray(DrawingState[0].idAttribTexture);
	glVertexAttribPointer(DrawingState[0].idAttribPosition, 2, GL_FLOAT, 0, 0, centroidVertices);
	glVertexAttribPointer(DrawingState[0].idAttribTexture, 2, GL_FLOAT, 0, 0, centroidTextures);
	glVertexAttrib4f(DrawingState[0].idAttribColor,1.0,1.0,1.0,1.0);
	glDisableVertexAttribArray(DrawingState[0].idAttribColor);
	
	framecomp=0.03*1/scale;
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	
	for(loopClass=0;loopClass<U->row;loopClass++)
	{
		
		auxCentroid[0] =  Centroids->data[loopClass*Centroids->col ];
		auxCentroid[1]=   Centroids->data[loopClass*Centroids->col+1 ];
		
		//left-up vertex
		centroidVertices[loopClass*8]  = auxCentroid[0]-framecomp;	 centroidTextures[loopClass*8]		=0;
		centroidVertices[loopClass*8+1]= auxCentroid[1]+framecomp;	 centroidTextures[loopClass*8 +1]	=1; 
		
		//left-down vertex
		centroidVertices[loopClass*8 +2]  = auxCentroid[0]-framecomp; centroidTextures[loopClass*8 +1]	=0;
		centroidVertices[loopClass*8 +3]= auxCentroid[1]-framecomp;   centroidTextures[loopClass*8 +1]	=0;
		
		//right-down vertex
		centroidVertices[loopClass*8 +4]  = auxCentroid[0]+framecomp; centroidTextures[loopClass*8 +1]	=1;
		centroidVertices[loopClass*8 +5]= auxCentroid[1]-framecomp;   centroidTextures[loopClass*8 +1]	=0;
		
		//right-up vertex
		centroidVertices[loopClass*8 +6]  = auxCentroid[0]+framecomp; centroidTextures[loopClass*8 +1]	=1;
		centroidVertices[loopClass*8 +7]= auxCentroid[1]+framecomp;   centroidTextures[loopClass*8 +1]	=1;
		
		glUniform1i(DrawingState[0].idTexture, PatternData[0].centroidText[loopClass]);
		//glDrawArrays(GL_QUADS, loopClass*4, 4);
	}
	
	glDisableVertexAttribArray(DrawingState[0].idAttribTexture);
	glDisableVertexAttribArray(DrawingState[0].idAttribColor);
	glDisableVertexAttribArray(DrawingState[0].idAttribPosition);	
	
	
	free(indices);
	free(vertices);
	free(colors);
	free(centroidVertices);
	free(centroidTextures);
	return 0;

}

int glplotHCMN(Matrix *Samples, Matrix *U, Matrix *Centroids)
{
	
	int i,j;
	float ranges[3*2];
	float translateCentroid[3]={0};
	float scales[3];
	float scale;
	float modelview[16];
	GLuint *indices=NULL;
	float *colors=NULL;
	float *vertices=NULL;
	float *cenCoords=NULL;
	
	int   loopClass;
	int   loopSample;
	int   loopDim;
	float modelRot[16];
	
	
	if(Samples==NULL || U==NULL)
		return -1;
	
	if(Samples->col==2 && Samples->col==3)
		return -2;
	
	
	indices= (GLuint*)malloc(sizeof(GLuint)*Samples->row);
	colors = (float*)malloc(sizeof(float)*Samples->row*3);
	vertices = (float*)malloc(sizeof(float)*Samples->row*Samples->col);
	
	memset(colors,0,sizeof(float)*Samples->row*3);
	
	for(loopDim=0;loopDim<Samples->col;loopDim++)
		ranges[loopDim*2+ 0]=ranges[loopDim*2+1]=Samples->data[loopDim];
	
	for(j=0;j<Samples->row;j++)
	{
		for(loopDim=0;loopDim<Samples->col;loopDim++)
		{
			vertices[j*Samples->col+loopDim ]=(float)(Samples->data[j*Samples->col +loopDim]);
			
			if(Samples->data[j*Samples->col +loopDim]<ranges[loopDim*2])
			{
				ranges[loopDim*2]=Samples->data[j*Samples->col +loopDim];
			}
			
			if(Samples->data[j*Samples->col +loopDim]>ranges[loopDim*2+1])
			{
				ranges[loopDim*2+1]=Samples->data[j*Samples->col +loopDim];
			}
		}
	}
	
	/*
	translateCentroid[0]=(rangeX[1]-rangeX[0])/2.0 + rangeX[0];
	translateCentroid[1]=(rangeY[1]-rangeY[0])/2.0 + rangeY[0];
	 */
	
	for(loopDim=0;loopDim<Samples->col;loopDim++)
	{
		translateCentroid[loopDim]=(ranges[loopDim*2+1] - ranges[loopDim*2])/2.0 + ranges[loopDim*2];
		scales[loopDim]=ranges[loopDim*2+1] - ranges[loopDim*2];
	}
	
	scale=scales[0];
	for(loopDim=0;loopDim<Samples->col;loopDim++)
		if(scale<scales[loopDim])
			scale=scales[loopDim];
	
	
	scale=1.85/scale;
	/*
	if(rangeX[1]-rangeX[0] > rangeY[1]-rangeY[0])
		scale = 1.85/(rangeX[1]-rangeX[0]);
	else
		scale = 1.85/(rangeY[1]-rangeY[0]);
	 */
	
	
	
	/*
	 scaleX = 1.85/(rangeX[1]-rangeX[0]);
	 scaleY = 1.85/(rangeY[1]-rangeY[0]);
	 */
	
	glPointSize(6);
	
	//Need fix this, the multiplication cannot be here, for simplicity
	matrixTranslateScale(-translateCentroid[0]*scale,-translateCentroid[1]*scale,-translateCentroid[2]*scale, scale, modelview);
	matrixMult(modelview, rotMatrix,modelRot);
	glUniformMatrix4fv(DrawingState[0].idModelview, 1, 0, modelRot);
	
	for(loopSample=0;loopSample<U->col;loopSample++)
	{
		for(loopClass=0;loopClass<U->row;loopClass++)
		{
			if(U->data[loopSample + loopClass*U->col]==1)
			{
				colors[loopSample*3]  =class_colors[loopClass][0];
				colors[loopSample*3+1]=class_colors[loopClass][1];
				colors[loopSample*3+2]=class_colors[loopClass][2];
				break;
			}
		}
	}
	
	glEnableVertexAttribArray(DrawingState[0].idAttribPosition);
	glEnableVertexAttribArray(DrawingState[0].idAttribColor);
	
	glDisableVertexAttribArray(DrawingState[0].idAttribTexture);
	glDisable(GL_TEXTURE_2D);
	
	glUniform1i(PatternData[0].plotPoints, 1); //Tells to the shader we now plot points, to avoid the texture part
	glVertexAttribPointer(DrawingState[0].idAttribPosition, Samples->col, GL_FLOAT, 0, 0, vertices);
	glVertexAttribPointer(DrawingState[0].idAttribColor, 3, GL_FLOAT, 0, 0, colors);
	
	//glDrawElements(GL_POINTS, Samples->row, GL_UNSIGNED_INT, indices);
	glDrawArrays(GL_POINTS, 0, Samples->row);
	
	
	if(Centroids!=NULL)
	{
		cenCoords=(float*)malloc(sizeof(float)*Centroids->row*Centroids->col);
	
		for(loopSample=0;loopSample<Centroids->row;loopSample++)
		{
			for(loopDim=0;loopDim<Centroids->col;loopDim++)
			{
				cenCoords[loopSample*Centroids->col + loopDim]=Centroids->data[loopSample*Centroids->col + loopDim];
			}
		}

		glPointSize(10);
		glDisable(GL_DEPTH_TEST);
	
		glVertexAttrib4f(DrawingState[0].idAttribColor, 0, 0, 0, 1.0);
		glDisableVertexAttribArray(DrawingState[0].idAttribColor);
		glVertexAttribPointer(DrawingState[0].idAttribPosition, Centroids->col, GL_FLOAT, 0, 0, cenCoords);
	
		glDrawArrays(GL_POINTS, 0, Centroids->row);
	
		glDisableVertexAttribArray(DrawingState[0].idAttribTexture);
		glDisableVertexAttribArray(DrawingState[0].idAttribColor);
		glDisableVertexAttribArray(DrawingState[0].idAttribPosition);
	
		glEnable(GL_DEPTH_TEST);
		free(cenCoords);
	}
	
	
	free(indices);
	free(vertices);
	free(colors);
	return 0;
	
}

int glplotScatter3D(Matrix *Samples)
{
	
	int i,j;
	float ranges[3*2];
	float translateCentroid[3]={0};
	float scales[3];
	float scale;
	float modelview[16];
	GLuint *indices=NULL;
	float *colors=NULL;
	float *vertices=NULL;
	int   loopClass;
	int   loopSample;
	int   loopDim;
	float modelRot[16];
	
	
	if(Samples==NULL)
		return -1;
	
	if(Samples->col==2 && Samples->col==3)
		return -2;
	
	
	indices= (GLuint*)malloc(sizeof(GLuint)*Samples->row);
	colors = (float*)malloc(sizeof(float)*Samples->row*3);
	vertices = (float*)malloc(sizeof(float)*Samples->row*Samples->col);
	
	memset(colors,0,sizeof(float)*Samples->row*3);
	
	for(loopDim=0;loopDim<Samples->col;loopDim++)
		ranges[loopDim*2+ 0]=ranges[loopDim*2+1]=Samples->data[loopDim];
	
	for(j=0;j<Samples->row;j++)
	{
		for(loopDim=0;loopDim<Samples->col;loopDim++)
		{
			vertices[j*Samples->col+loopDim ]=(float)(Samples->data[j*Samples->col +loopDim]);
			
			if(Samples->data[j*Samples->col +loopDim]<ranges[loopDim*2])
			{
				ranges[loopDim*2]=Samples->data[j*Samples->col +loopDim];
			}
			
			if(Samples->data[j*Samples->col +loopDim]>ranges[loopDim*2+1])
			{
				ranges[loopDim*2+1]=Samples->data[j*Samples->col +loopDim];
			}
		}
	}
	
	/*
	 translateCentroid[0]=(rangeX[1]-rangeX[0])/2.0 + rangeX[0];
	 translateCentroid[1]=(rangeY[1]-rangeY[0])/2.0 + rangeY[0];
	 */
	
	for(loopDim=0;loopDim<Samples->col;loopDim++)
	{
		translateCentroid[loopDim]=(ranges[loopDim*2+1] - ranges[loopDim*2])/2.0 + ranges[loopDim*2];
		scales[loopDim]=ranges[loopDim*2+1] - ranges[loopDim*2];
	}
	
	scale=scales[0];
	for(loopDim=0;loopDim<Samples->col;loopDim++)
		if(scale<scales[loopDim])
			scale=scales[loopDim];
	
	
	scale=1.85/scale;
	/*
	 if(rangeX[1]-rangeX[0] > rangeY[1]-rangeY[0])
	 scale = 1.85/(rangeX[1]-rangeX[0]);
	 else
	 scale = 1.85/(rangeY[1]-rangeY[0]);
	 */
	
	
	
	/*
	 scaleX = 1.85/(rangeX[1]-rangeX[0]);
	 scaleY = 1.85/(rangeY[1]-rangeY[0]);
	 */
	
	glPointSize(6);
	
	//Need fix this, the multiplication cannot be here, for simplicity
	matrixTranslateScale(-translateCentroid[0]*scale,-translateCentroid[1]*scale,-translateCentroid[2]*scale, scale, modelview);
	
	if(AutoSet!=1)
		matrixIdentity(modelview);
	
	matrixMult(modelview, rotMatrix,modelRot);
	
	glUniformMatrix4fv(DrawingState[0].idModelview, 1, 0, modelRot);
	
	for(loopSample=0;loopSample<Samples->row;loopSample++)
	{
		colors[loopSample*3]=scatterColor[0];
		colors[loopSample*3+1]=scatterColor[1];
		colors[loopSample*3+2]=scatterColor[2];
		
	}
	
	glEnableVertexAttribArray(DrawingState[0].idAttribPosition);
	glEnableVertexAttribArray(DrawingState[0].idAttribColor);
	
	glDisableVertexAttribArray(DrawingState[0].idAttribTexture);
	glDisable(GL_TEXTURE_2D);
	
	glUniform1i(PatternData[0].plotPoints, 1); //Tells to the shader we now plot points, to avoid the texture part
	glVertexAttribPointer(DrawingState[0].idAttribPosition, Samples->col, GL_FLOAT, 0, 0, vertices);
	glVertexAttribPointer(DrawingState[0].idAttribColor, 3, GL_FLOAT, 0, 0, colors);
	
	//glDrawElements(GL_POINTS, Samples->row, GL_UNSIGNED_INT, indices);
	glDrawArrays(GL_POINTS, 0, Samples->row);
	
	glDisableVertexAttribArray(DrawingState[0].idAttribTexture);
	glDisableVertexAttribArray(DrawingState[0].idAttribColor);
	glDisableVertexAttribArray(DrawingState[0].idAttribPosition);	
	
	free(indices);
	free(vertices);
	free(colors);
	return 0;
	
}

int	glplotLineStrip(Matrix  *Samples)
{	
	int i,j;
	float ranges[3*2];
	float translateCentroid[3]={0};
	float scales[3];
	float scale;
	float modelview[16];
	GLuint *indices=NULL;
	float *colors=NULL;
	float *vertices=NULL;
	int   loopClass;
	int   loopSample;
	int   loopDim;
	float modelRot[16];
	
	
	if(Samples==NULL)
		return -1;
	
	if(Samples->col==2 && Samples->col==3)
		return -2;
	
	if(Samples->row<1)
		return -3;
	
	indices= (GLuint*)malloc(sizeof(GLuint)*Samples->row);
	colors = (float*)malloc(sizeof(float)*Samples->row*3);
	vertices = (float*)malloc(sizeof(float)*Samples->row*Samples->col);
	
	//printf("Pidiendo memoria\n");

	if(indices==NULL || colors==NULL || vertices==NULL)
	{
		printf("Error en alojar memoria\n");
		return -4;
	}

	memset(colors,0,sizeof(float)*Samples->row*3);

	//printf("Ajustando memoria\n");
	
	for(loopDim=0;loopDim<Samples->col;loopDim++)
	{
		ranges[loopDim*2+ 0]=ranges[loopDim*2+1]=Samples->data[loopDim];
		//printf("%i, %i\n",loopDim, Samples->col);
	}

	//printf("Obteniendo Ranges");
	
	for(j=0;j<Samples->row;j++)
	{
		for(loopDim=0;loopDim<Samples->col;loopDim++)
		{
			vertices[j*Samples->col+loopDim ]=(float)(Samples->data[j*Samples->col +loopDim]);
			
			if(Samples->data[j*Samples->col +loopDim]<ranges[loopDim*2])
			{
				ranges[loopDim*2]=Samples->data[j*Samples->col +loopDim];
			}
			
			if(Samples->data[j*Samples->col +loopDim]>ranges[loopDim*2+1])
			{
				ranges[loopDim*2+1]=Samples->data[j*Samples->col +loopDim];
			}
		}
	}

	//printf("Obteniendo Rangos\n");
	
	/*
	 translateCentroid[0]=(rangeX[1]-rangeX[0])/2.0 + rangeX[0];
	 translateCentroid[1]=(rangeY[1]-rangeY[0])/2.0 + rangeY[0];
	 */
	
	for(loopDim=0;loopDim<Samples->col;loopDim++)
	{
		translateCentroid[loopDim]=(ranges[loopDim*2+1] - ranges[loopDim*2])/2.0 + ranges[loopDim*2];
		scales[loopDim]=ranges[loopDim*2+1] - ranges[loopDim*2];
	}
	
	scale=scales[0];
	for(loopDim=0;loopDim<Samples->col;loopDim++)
		if(scale<scales[loopDim])
			scale=scales[loopDim];
	
	
	scale=1.85/scale;
	/*
	 if(rangeX[1]-rangeX[0] > rangeY[1]-rangeY[0])
	 scale = 1.85/(rangeX[1]-rangeX[0]);
	 else
	 scale = 1.85/(rangeY[1]-rangeY[0]);
	 */
	
	
	
	/*
	 scaleX = 1.85/(rangeX[1]-rangeX[0]);
	 scaleY = 1.85/(rangeY[1]-rangeY[0]);
	 */
	
	//printf("Point Size\n");
	glPointSize(6);
	//printf("Pasando point size\n");	

	//Need fix this, the multiplication cannot be here, for simplicity
	matrixTranslateScale(-translateCentroid[0]*scale,-translateCentroid[1]*scale,-translateCentroid[2]*scale, scale, modelview);
	
	if(AutoSet!=1)
		matrixIdentity(modelview);
	
	matrixMult(modelview, rotMatrix,modelRot);
	
		
	glUniformMatrix4fv(DrawingState[0].idModelview, 1, 0, modelRot);
	
	for(loopSample=0;loopSample<Samples->row;loopSample++)
	{
		colors[loopSample*3]=scatterColor[0];
		colors[loopSample*3+1]=scatterColor[1];
		colors[loopSample*3+2]=scatterColor[2];
	}
	
	glEnableVertexAttribArray(DrawingState[0].idAttribPosition);
	glEnableVertexAttribArray(DrawingState[0].idAttribColor);
	
	glDisableVertexAttribArray(DrawingState[0].idAttribTexture);
	glDisable(GL_TEXTURE_2D);
	
	glUniform1i(PatternData[0].plotPoints, 1); //Tells to the shader we now plot points, to avoid the texture part
	glVertexAttribPointer(DrawingState[0].idAttribPosition, Samples->col, GL_FLOAT, 0, 0, vertices);
	glVertexAttribPointer(DrawingState[0].idAttribColor, 3, GL_FLOAT, 0, 0, colors);
	
	//glDrawElements(GL_POINTS, Samples->row, GL_UNSIGNED_INT, indices);
	glDrawArrays(GL_LINE_STRIP, 0, Samples->row);
	
	glDisableVertexAttribArray(DrawingState[0].idAttribTexture);
	glDisableVertexAttribArray(DrawingState[0].idAttribColor);
	glDisableVertexAttribArray(DrawingState[0].idAttribPosition);	
	
	free(indices);
	free(vertices);
	free(colors);
	return 0;
}

int	 glplotScatter2D(Matrix *Samples)
{
	int i,j;
	float rangeX[2];
	float rangeY[2];
	float translateCentroid[2];
	float scale;
	float modelview[16];
	GLuint *indices=NULL;
	float *colors=NULL;
	float *vertices=NULL;
	
	if(Samples==NULL)
		return -1;
	
	if(Samples->col!=2)
		return -2;
	
	
	indices= (GLuint*)malloc(sizeof(GLuint)*Samples->row);
	colors = (float*)malloc(sizeof(float)*Samples->row*3);
	vertices = (float*)malloc(sizeof(float)*Samples->row*2);
	
	memset(colors,0,sizeof(float)*Samples->row*3);
	
	rangeX[1]=rangeX[0]=Samples->data[0];
	rangeY[1]=rangeY[0]=Samples->data[1];
	
	
	for(i=0,j=0;j<Samples->row;i+=2,j++)
	{
		if(Samples->data[i] <rangeX[0] )
		{
			rangeX[0]=Samples->data[i];
		}
		
		if(Samples->data[i] >rangeX[1])
		{
			rangeX[1]=Samples->data[i];
		}
		
		if(Samples->data[i+1] <rangeY[0] )
		{
			rangeY[0]=Samples->data[i+1];
		}
		
		if(Samples->data[i+1]>rangeY[1])
		{
			rangeY[1]=Samples->data[i+1];
		}
		
		indices[j]=j;
		vertices[i]=(float)(Samples->data[i]);
		vertices[i+1]=(float)(Samples->data[i+1]);
	}
	
	translateCentroid[0]=(rangeX[1]-rangeX[0])/2.0 + rangeX[0];
	translateCentroid[1]=(rangeY[1]-rangeY[0])/2.0 + rangeY[0];
	
	
	if(rangeX[1]-rangeX[0] > rangeY[1]-rangeY[0])
		scale = 1.85/(rangeX[1]-rangeX[0]);
	else
		scale = 1.85/(rangeY[1]-rangeY[0]);
	
	/*
	 scaleX = 1.85/(rangeX[1]-rangeX[0]);
	 scaleY = 1.85/(rangeY[1]-rangeY[0]);
	 */
	
	glPointSize(6);
	
	//Need fix this, the multiplication cannot be here, for simplicity
	matrixTranslateScale(-translateCentroid[0]*scale,-translateCentroid[1]*scale,0, scale, modelview);
	glUniformMatrix4fv(DrawingState[0].idModelview, 1, 0, modelview);
	
	memset(colors, 0, sizeof(float)*Samples->row*3);
	
	glEnableVertexAttribArray(DrawingState[0].idAttribPosition);
	glEnableVertexAttribArray(DrawingState[0].idAttribColor);	
	glDisableVertexAttribArray(DrawingState[0].idAttribTexture);
	
	glUniform1i(PatternData[0].plotPoints, 1); //Tells to the shader we now plot points, to avoid the texture part
	glVertexAttribPointer(DrawingState[0].idAttribPosition, 2, GL_FLOAT, 0, 0, vertices);
	glVertexAttribPointer(DrawingState[0].idAttribColor, 3, GL_FLOAT, 0, 0, colors);
	
	glDrawElements(GL_POINTS, Samples->row, GL_UNSIGNED_INT, indices);
	
	glDisableVertexAttribArray(DrawingState[0].idAttribPosition);
	glDisableVertexAttribArray(DrawingState[0].idAttribColor);
	glDisableVertexAttribArray(DrawingState[0].idAttribTexture);
	
	free(vertices);
	free(colors);
	free(indices);
	
	return 0;
	
}



int	  glplotTestTexture()
{
	float vertices[]={	0.0,0.0,0.0,	1.0,0.0,0.0,	0.0,1.0,0.0,	1.0,0.0,0.0,	1.0,1.0,0.0,	0.0,1.0,0.0};
	float texcoords[]={	0.0,0.0,		1.0,0.0,		0.0,1.0,		1.0,0.0,		1.0,1.0,		0.0,1.0};
	float colors[]={	1.0,0.0,0.0,	1.0,0.0,0.0,	1.0,0.0,0.0,	1.0,0.0,0.0,	1.0,0.0,0.0,	1.0,0.0,0.0};
	float iden[16];
	
	glEnableVertexAttribArray(DrawingState[0].idAttribPosition);
	glEnableVertexAttribArray(DrawingState[0].idAttribTexture);
	glEnableVertexAttribArray(DrawingState[0].idAttribColor);
	
	matrixIdentity(iden);	
	matrixTranslateScale(-0.5, -0.5, 0.0, 0.08, iden);
	glUniformMatrix4fv(DrawingState[0].idModelview,1, 0, iden);
	glUniform1i(PatternData[0].plotPoints, 0);
	
	glEnable(GL_TEXTURE_2D);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, PatternData[0].centroidText[7]);
	glUniform1i (DrawingState[0].idTexture, 0 );
	
	glVertexAttribPointer(DrawingState[0].idAttribPosition, 3, GL_FLOAT, 0, 0, vertices);
	glVertexAttribPointer(DrawingState[0].idAttribColor, 3, GL_FLOAT, 0, 0, colors);
	glVertexAttribPointer(DrawingState[0].idAttribTexture, 2, GL_FLOAT, 0, 0, texcoords);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	glDisableVertexAttribArray(DrawingState[0].idAttribPosition);
	glDisableVertexAttribArray(DrawingState[0].idAttribTexture);
	glDisableVertexAttribArray(DrawingState[0].idAttribColor);
	

	glDisable(GL_TEXTURE_2D);
	return 0;
}

int	glplotGetIDTexture(int indexTexture)
{
	if(indexTexture>=N_CTEXT)
		return -1;
	return PatternData[0].centroidText[indexTexture];
}

int	glplotGetPlotStats(PlotStats **status)
{
	if(status==NULL)
		return PLOT_ERR_ARG;
	
	*status=DrawingState;
	
	return 0;
}


int glplotScatterColor(float r,float g, float b)
{
	scatterColor[0]=r;
	scatterColor[1]=g;
	scatterColor[2]=b;
	
	return 0;
}


//Azimutal it's along the ecuator (x to -x), polar it's between poles(y to -y)
int glplotApplyRotations(float polar,float azimutal)
{
	float rotPole[16],rotAzi[16];
	float rotResult[16]={0};
	float sinPolar,cosPolar;
	float sinAzi,cosAzi;
	
	matrixIdentity(rotPole);
	matrixIdentity(rotAzi);
	
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
	
	matrixMult(rotPole, rotAzi, rotResult);
	
	memcpy(rotMatrix,rotResult ,sizeof(float)*16);
	return 0;
}

int matrixRotate(float polar,float azimutal, float *m)
{
	float rotPole[16],rotAzi[16];
	float rotResult[16]={0};
	float sinPolar,cosPolar;
	float sinAzi,cosAzi;
	
	matrixIdentity(rotPole);
	matrixIdentity(rotAzi);
	
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
	
	matrixMult(rotPole, rotAzi, rotResult);
	
	memcpy(m,rotResult ,sizeof(float)*16);
	return 0;
}

int		glplotBasicAssetCreate(PlotBasicAsset **asset, int nvertices, int ncolors, int ntexcoord,int nnormals ,int nindex)
{
	PlotBasicAsset *temp_asset=NULL;
	
	if(asset==NULL || nvertices<=0 || nindex<=0)
	{
		return MAT_ERR_ARG;
	}
	
	temp_asset=(PlotBasicAsset*)malloc(sizeof(PlotBasicAsset));
	memset(temp_asset,0,sizeof(PlotBasicAsset));
	
	if(temp_asset==NULL)
		return MAT_ERR_MEM;
	

	temp_asset->nVertices=nvertices;
	temp_asset->vertices=(float*)malloc(sizeof(float)*nvertices*3);
	
	if(temp_asset->vertices==NULL)
		return MAT_ERR_MEM;
	
	temp_asset->nindex=nindex;
	temp_asset->index=(GLushort*)malloc(sizeof(GLushort)*nindex);
	
	if(temp_asset->index==NULL)
	{
		free(temp_asset->vertices);
		return MAT_ERR_MEM;
	}
	
	if(ncolors>0)
	{
		temp_asset->nColors=ncolors;
		temp_asset->colorsRGB=(float*)malloc(sizeof(float)*ncolors*3);
		
		if(temp_asset->colorsRGB==NULL)
		{
			free(temp_asset->index);
			free(temp_asset->vertices);
			 return MAT_ERR_MEM;
		}
	}
	
	if(ntexcoord>0)
	{
		temp_asset->nTexcoord=ncolors;
		temp_asset->textcoord2D=(float*)malloc(sizeof(float)*ntexcoord*2);
		
		if(temp_asset->textcoord2D==NULL)
		{
			free(temp_asset->index);
			free(temp_asset->vertices);
			
			if(temp_asset->colorsRGB!=NULL)
				free(temp_asset->colorsRGB);
			
			
			return MAT_ERR_MEM;
		}
	}
	
	if(nnormals>0)
	{
		temp_asset->nNormals=nnormals;
		temp_asset->normals=(float*)malloc(sizeof(float)*nnormals*3);
		
		if(temp_asset->normals==NULL)
		{
			free(temp_asset->index);
			free(temp_asset->vertices);
			
			if(temp_asset->colorsRGB!=NULL)
				free(temp_asset->colorsRGB);
			
			if(temp_asset->textcoord2D!=NULL)
				free(temp_asset->textcoord2D);
			
			return MAT_ERR_MEM;
		}
	}
	
	matrixIdentity(temp_asset->matrix_modelview);
	
	*asset=temp_asset;
	return 0;
}

int		glplotBasicAssetDetelete(PlotBasicAsset **asset)
{
	PlotBasicAsset *temp=NULL;
	
	if(asset==NULL)
		return MAT_ERR_ARG;
	
	temp=*asset;

	if(temp->vertices!=NULL)
		free(temp->vertices);
	
	if(temp->colorsRGB!=NULL)
		free(temp->colorsRGB);
	
	if(temp->textcoord2D!=NULL)
		free(temp->textcoord2D);
	
	if(temp->index!=NULL)
		free(temp->index);
	
	free(temp);
	
	*asset=NULL;
	return 0;
}

int	glplotBasicAssetCube(PlotBasicAsset **asset,float edge,float *color_v)
{
	PlotBasicAsset *temp=NULL;
	int res;
	float *ptr=NULL;
	float *ptrNormal=NULL;
	float *ptrTex=NULL;
	int i;
	u_Material *ptrMaterial=NULL;
	
	if(asset==NULL || edge<=0 )
		return PLOT_ERR_ARG;
	
	res = glplotBasicAssetCreate(&temp,3*2*6, 3*2*6, 3*2*6, 2*3*6,2*3*6);
	
	if(res<0)
		return res;
	
	ptr=temp->vertices;
	ptrNormal=temp->normals;
	ptrTex=temp->textcoord2D;
	//front face - vertices								---		normals																		---		texture
	ptr[0]=0.0f; ptr[1]=edge; ptr[2]=0.0f; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=0.0f; ptrNormal[2]=1.0f; ptrNormal+=3;				ptrTex[0]=0.0; ptrTex[1]=1.0f; ptrTex+=2;
	ptr[0]=0.0f; ptr[1]=0.0f; ptr[2]=0.0f; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=0.0f; ptrNormal[2]=1.0f; ptrNormal+=3;				ptrTex[0]=0.0; ptrTex[1]=0.0f; ptrTex+=2;
	ptr[0]=edge; ptr[1]=0.0f; ptr[2]=0.0f; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=0.0f; ptrNormal[2]=1.0f; ptrNormal+=3;				ptrTex[0]=1.0; ptrTex[1]=0.0f; ptrTex+=2;
	
	ptr[0]=edge; ptr[1]=edge; ptr[2]=0.0f; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=0.0f; ptrNormal[2]=1.0f; ptrNormal+=3;				ptrTex[0]=1.0; ptrTex[1]=1.0f; ptrTex+=2;
	ptr[0]=0.0f; ptr[1]=edge; ptr[2]=0.0f; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=0.0f; ptrNormal[2]=1.0f; ptrNormal+=3;				ptrTex[0]=0.0; ptrTex[1]=1.0f; ptrTex+=2;
	ptr[0]=edge; ptr[1]=0.0f; ptr[2]=0.0f; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=0.0f; ptrNormal[2]=1.0f; ptrNormal+=3;				ptrTex[0]=1.0; ptrTex[1]=0.0f; ptrTex+=2;
	
	//right face
	ptr[0]=edge; ptr[1]=edge; ptr[2]=0.0f; ptr+=3;				ptrNormal[0]=1.0f; ptrNormal[1]=0.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=0.0; ptrTex[1]=1.0f; ptrTex+=2;
	ptr[0]=edge; ptr[1]=0.0f; ptr[2]=0.0f; ptr+=3;				ptrNormal[0]=1.0f; ptrNormal[1]=0.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=0.0; ptrTex[1]=0.0f; ptrTex+=2;
	ptr[0]=edge; ptr[1]=0.0f; ptr[2]=-edge; ptr+=3;				ptrNormal[0]=1.0f; ptrNormal[1]=0.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=1.0; ptrTex[1]=0.0f; ptrTex+=2;
		
	ptr[0]=edge; ptr[1]=edge; ptr[2]=0.0f; ptr+=3;				ptrNormal[0]=1.0f; ptrNormal[1]=0.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=0.0; ptrTex[1]=1.0f; ptrTex+=2;
	ptr[0]=edge; ptr[1]=0.0f; ptr[2]=-edge; ptr+=3;				ptrNormal[0]=1.0f; ptrNormal[1]=0.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=1.0; ptrTex[1]=0.0f; ptrTex+=2;
	ptr[0]=edge; ptr[1]=edge; ptr[2]=-edge; ptr+=3;				ptrNormal[0]=1.0f; ptrNormal[1]=0.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=1.0; ptrTex[1]=1.0f; ptrTex+=2;
	
	//back face
	ptr[0]=edge; ptr[1]=0.0f; ptr[2]=-edge; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=0.0f; ptrNormal[2]=-1.0f; ptrNormal+=3;				ptrTex[0]=1.0; ptrTex[1]=0.0f; ptrTex+=2;
	ptr[0]=0.0f; ptr[1]=0.0f; ptr[2]=-edge; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=0.0f; ptrNormal[2]=-1.0f; ptrNormal+=3;				ptrTex[0]=0.0; ptrTex[1]=0.0f; ptrTex+=2;
	ptr[0]=0.0f; ptr[1]=edge; ptr[2]=-edge; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=0.0f; ptrNormal[2]=-1.0f; ptrNormal+=3;				ptrTex[0]=0.0; ptrTex[1]=1.0f; ptrTex+=2;
	
	ptr[0]=edge; ptr[1]=0.0f; ptr[2]=-edge; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=0.0f; ptrNormal[2]=-1.0f; ptrNormal+=3;				ptrTex[0]=1.0; ptrTex[1]=0.0f; ptrTex+=2;
	ptr[0]=0.0f; ptr[1]=edge; ptr[2]=-edge; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=0.0f; ptrNormal[2]=-1.0f; ptrNormal+=3;				ptrTex[0]=0.0; ptrTex[1]=1.0f; ptrTex+=2;
	ptr[0]=edge; ptr[1]=edge; ptr[2]=-edge; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=0.0f; ptrNormal[2]=-1.0f; ptrNormal+=3;				ptrTex[0]=1.0; ptrTex[1]=1.0f; ptrTex+=2;
	
	
	//left face
	ptr[0]=0.0f; ptr[1]=0.0f; ptr[2]=-edge; ptr+=3;				ptrNormal[0]=-1.0f; ptrNormal[1]=0.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=0.0; ptrTex[1]=0.0f; ptrTex+=2;
	ptr[0]=0.0f; ptr[1]=0.0f; ptr[2]= 0.0f; ptr+=3;				ptrNormal[0]=-1.0f; ptrNormal[1]=0.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=1.0; ptrTex[1]=0.0f; ptrTex+=2;
	ptr[0]=0.0f; ptr[1]=edge; ptr[2]=0.0f; ptr+=3;				ptrNormal[0]=-1.0f; ptrNormal[1]=0.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=1.0; ptrTex[1]=1.0f; ptrTex+=2;
	
	ptr[0]=0.0f; ptr[1]=edge; ptr[2]=-edge; ptr+=3;				ptrNormal[0]=-1.0f; ptrNormal[1]=0.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=0.0; ptrTex[1]=1.0f; ptrTex+=2;
	ptr[0]=0.0f; ptr[1]=0.0f; ptr[2]=-edge; ptr+=3;				ptrNormal[0]=-1.0f; ptrNormal[1]=0.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=0.0; ptrTex[1]=0.0f; ptrTex+=2;
	ptr[0]=0.0f; ptr[1]=edge; ptr[2]=0.0f; ptr+=3;				ptrNormal[0]=-1.0f; ptrNormal[1]=0.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=1.0; ptrTex[1]=1.0f; ptrTex+=2;
	
	//top
	ptr[0]=0.0f; ptr[1]=edge; ptr[2]=-edge; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=1.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=0.0; ptrTex[1]=1.0f; ptrTex+=2;
	ptr[0]=0.0f; ptr[1]=edge; ptr[2]= 0.0f; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=1.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=0.0; ptrTex[1]=0.0f; ptrTex+=2;
	ptr[0]=edge; ptr[1]=edge; ptr[2]=0.0f; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=1.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=1.0; ptrTex[1]=0.0f; ptrTex+=2;
	
	ptr[0]=0.0f; ptr[1]=edge; ptr[2]=-edge; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=1.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=0.0; ptrTex[1]=1.0f; ptrTex+=2;
	ptr[0]=edge; ptr[1]=edge; ptr[2]=0.0f; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=1.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=1.0; ptrTex[1]=0.0f; ptrTex+=2;
	ptr[0]=edge; ptr[1]=edge; ptr[2]=-edge; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=1.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=1.0; ptrTex[1]=1.0f; ptrTex+=2;
	
	
	//bottom
	ptr[0]=edge; ptr[1]=0.0f; ptr[2]=0.0f; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=-1.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=1.0; ptrTex[1]=0.0f; ptrTex+=2;
	ptr[0]=0.0f; ptr[1]=0.0f; ptr[2]= 0.0f; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=-1.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=0.0; ptrTex[1]=0.0f; ptrTex+=2;
	ptr[0]=0.0f; ptr[1]=0.0f; ptr[2]=-edge; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=-1.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=0.0; ptrTex[1]=1.0f; ptrTex+=2;

	ptr[0]=edge; ptr[1]=0.0f; ptr[2]=-edge; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=-1.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=1.0; ptrTex[1]=1.0f; ptrTex+=2;
	ptr[0]=edge; ptr[1]=0.0f; ptr[2]=0.0f; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=-1.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=1.0; ptrTex[1]=0.0f; ptrTex+=2;
	ptr[0]=0.0f; ptr[1]=0.0f; ptr[2]=-edge; ptr+=3;				ptrNormal[0]=0.0f; ptrNormal[1]=-1.0f; ptrNormal[2]=0.0f; ptrNormal+=3;				ptrTex[0]=0.0; ptrTex[1]=1.0f; ptrTex+=2;
	
	//printf("Indices escritos: %i", (ptr - temp->vertices)/3);
	for(i=0;i<3*2*6;i++)
	{
		temp->vertices[i*3]-=edge/2.0f;
		temp->vertices[i*3+1]-=edge/2.0f;
		temp->vertices[i*3+2]+=edge/2.0f;
		
		//printf("Cara %i: %f, %f, %f\n",i,temp->vertices[i*3],temp->vertices[i*3+1],temp->vertices[i*3+2]);
	}

	if(color_v!=NULL)
	{
		for(i=0;i<temp->nColors;i++)
		{
			temp->colorsRGB[3*i]=color_v[0];
			temp->colorsRGB[3*i+1]=color_v[1];
			temp->colorsRGB[3*i+2]=color_v[2];
		}
	}
	else
	{
		memset(temp->colorsRGB, 0, sizeof(float)*3*temp->nColors);
	}
	
	for(i=0;i<temp->nindex;i++)
		temp->index[i]=i;
	
	*asset=temp;
	
	//set default values for lighitng
	ptrMaterial=&(temp->assetMaterial);
	
	ptrMaterial->ambientFactor[0]=0.9;
	ptrMaterial->ambientFactor[1]=0.9;
	ptrMaterial->ambientFactor[2]=0.9;
	ptrMaterial->ambientFactor[3]=1.0;
	
	ptrMaterial->diffuseFactor[0]=0.1;
	ptrMaterial->diffuseFactor[1]=0.7;
	ptrMaterial->diffuseFactor[2]=0.1;
	ptrMaterial->diffuseFactor[3]=0.7;

	ptrMaterial->shininess=0.8;
	
	ptrMaterial->specularFactor[0]=0.9;
	ptrMaterial->specularFactor[1]=0.9;
	ptrMaterial->specularFactor[2]=0.9;
	ptrMaterial->specularFactor[3]=0.9;
	return 0;
}

int	glplotBasicAssetPlane(PlotBasicAsset **asset,float *pi, float *pf,int segments)
{
	PlotBasicAsset *temp;
	int i,j;
	float *ptrVertex=NULL,*ptrColor=NULL,*ptrText=NULL,*ptrNormal=NULL;
	
	if(asset==NULL ||pi==NULL || pf==NULL || segments<=0 )
		return PLOT_ERR_ARG;
	
	glplotBasicAssetCreate(&temp,(segments+2)*(segments+2), 
								 (segments+2)*(segments+2), 
								 (segments+2)*(segments+2),
								(segments+2)*(segments+2), 3*2*(segments-1)*(segments-1));
			
	ptrVertex=temp->vertices;
	for(i=0;i<segments;i++)
	{
		for(j=0;j<segments;j++,ptrVertex+=3)
		{
			ptrVertex[0]=(pf[0]-pi[0])*j/((float)segments); ptrVertex[1]=0.0f; ptrVertex[2]=0.0f;
		}
	}
	
	return 0;
}

int	glplotBasicAsset(PlotBasicAsset *asset)
{
	u_Material *ptrMaterial=NULL;
	float      minv[16];
	float      mtras[16];
	
	if(asset==NULL)
		return PLOT_ERR_ARG;
	
	glUseProgram(DrawingState[0].programHandle);
	
	ptrMaterial=&(asset->assetMaterial);
	
	if(asset->normals!=NULL)
	{
		glEnableVertexAttribArray(DrawingState[0].idAttribNormal);
		glVertexAttribPointer(DrawingState[0].idAttribNormal, 3, GL_FLOAT, 0, 0, asset->normals);
		
		glUniform4fv(loc_material.ambientFactor, 1, ptrMaterial->ambientFactor);
		glUniform4fv(loc_material.diffuseFactor, 1, ptrMaterial->diffuseFactor);
		glUniform1f(loc_material.shininess,			 ptrMaterial->shininess);
		glUniform4fv(loc_material.specularFactor, 1, ptrMaterial->specularFactor);
		
		glUniform4fv(loc_directionalLight.ambientColor	,1,u_directionalLight.ambientColor);
		glUniform4fv(loc_directionalLight.diffuseColor	,1,u_directionalLight.diffuseColor);
		glUniform3fv(loc_directionalLight.direction		,1,u_directionalLight.direction);
		glUniform3fv(loc_directionalLight.halfplane		,1,u_directionalLight.halfplane);
		glUniform4fv(loc_directionalLight.specularColor	,1,u_directionalLight.specularColor);
		
		matrixInverse(asset->matrix_modelview, minv);
		matrixTransponse(minv, mtras);
		
		glUniformMatrix4fv(DrawingState[0].idNormalMat, 1, 0, mtras);
	}
		
	if(asset->textcoord2D!=NULL)
	{
		glEnableVertexAttribArray(DrawingState[0].idAttribTexture);
		glVertexAttribPointer(DrawingState[0].idAttribTexture, 2, GL_FLOAT, 0, 0, asset->textcoord2D);
	}
	
	glEnableVertexAttribArray(DrawingState[0].idAttribPosition);
	glEnableVertexAttribArray(DrawingState[0].idAttribColor);
	
	glUniformMatrix4fv(DrawingState[0].idModelview,1, 0, asset->matrix_modelview);
	glUniform1i(PatternData[0].plotPoints, 0);
	
	glEnable(GL_TEXTURE_2D);
	
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(DrawingState[0].idTexture, 0 );

	//printf("IDT: %i",DrawingState[0].idTexture);
	
	glVertexAttribPointer(DrawingState[0].idAttribPosition, 3, GL_FLOAT, 0, 0, asset->vertices);
	glVertexAttribPointer(DrawingState[0].idAttribColor, 3, GL_FLOAT, 0, 0, asset->colorsRGB);
	
	glDrawElements(GL_TRIANGLES, asset->nindex, GL_UNSIGNED_SHORT, asset->index);
	
	glDisableVertexAttribArray(DrawingState[0].idAttribPosition);
	glDisableVertexAttribArray(DrawingState[0].idAttribTexture);
	glDisableVertexAttribArray(DrawingState[0].idAttribColor);
	glDisableVertexAttribArray(DrawingState[0].idAttribNormal);
	
	glDisable(GL_TEXTURE_2D);
	
	return 0;
}

void glplotSetProjectionMatrix(float *m4)
{
	if(m4==NULL)
		return;
	
	glUniformMatrix4fv(DrawingState[0].idProjection, 1, 0, m4);
	printf("GLerror: %i\n",glGetError());
}

int	 glplotAutoset(int enable)
{
	AutoSet=enable;
	return 0;
}
