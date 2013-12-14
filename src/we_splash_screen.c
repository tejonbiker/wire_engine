#include "we_splash_screen.h"

static int ready=0;
static BaseShader splash_shaders;
static GLuint textures_id[13]={0};
static float  proj_buffer[16];

static char   textureNames[13][256]={
{"textures/wire_engine/w_letter.png"},
{"textures/wire_engine/i_letter.png"},
{"textures/wire_engine/r_letter.png"},
{"textures/wire_engine/e_letter.png"},
{"textures/wire_engine/engine_letter.png"},
{"textures/wire_engine/power_letter.png"},
{"textures/wire_engine/arrow.png"},

{"textures/logo_text/gba_text_256.png"},
{"textures/logo_text/snes_text.jpg"},
{"textures/logo_text/psone_text.jpg"},
{"textures/logo_text/snes.jpg"},
{"textures/logo_text/gba.jpg"},
{"textures/logo_text/psone.png"}
};

static WEQuad  WireArray[13];

typedef struct tagSplashVars{
	GLint Projection;
	GLint Modelview;
	GLint Sampler;
	GLint Colors;
	GLint Vertex;
	GLint Texture;
}SplashVars;

static WEShaderVars shader_vars={0};
static SplashVars splashVars={0};

static CatmullArray anim_position,anim_scale;
static WEQuadAnim   quad_anim;

int     weDrawQuad(WEQuad *object,float *mv)
{	
	glEnableVertexAttribArray(splashVars.Colors);
	glEnableVertexAttribArray(splashVars.Texture);
	glEnableVertexAttribArray(splashVars.Vertex);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,object->texture_id);

	glUniform1i(splashVars.Sampler,0);

	glVertexAttribPointer(splashVars.Colors,3,GL_FLOAT,0,0,object->color);
	glVertexAttribPointer(splashVars.Texture,2,GL_FLOAT,0,0,object->texture);
	glVertexAttribPointer(splashVars.Vertex,3,GL_FLOAT,0,0,object->vertex);

	glUniformMatrix4fv(splashVars.Projection,1,0,proj_buffer);
	glUniformMatrix4fv(splashVars.Modelview,1,0,mv);
	

	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,object->indices);

	glDisableVertexAttribArray(splashVars.Colors);
	glDisableVertexAttribArray(splashVars.Texture);
	glDisableVertexAttribArray(splashVars.Vertex);
}

int     queryVars()
{
	GLint *ptr=NULL;
	int i;

	printf("Enterign in quey... \n");

	ptr=&(splashVars.Projection);
	
	splashVars.Projection=glGetUniformLocation(splash_shaders.program,"Projection");
	splashVars.Modelview=glGetUniformLocation(splash_shaders.program,"Modelview");
	splashVars.Vertex=glGetAttribLocation(splash_shaders.program,"Vertex");
	splashVars.Colors=glGetAttribLocation(splash_shaders.program,"Colors");	
	splashVars.Texture=glGetAttribLocation(splash_shaders.program,"Texture");
	splashVars.Sampler=glGetUniformLocation(splash_shaders.program,"unitTexture");


	for(i=0;i<sizeof(SplashVars)/sizeof(GLint);i++,ptr++)
	{
		if(*ptr<0)
		{
			printf("Shader: Error to query a variable: %i\n",i);
			return -1;
		}
		else
		{
			printf("Query successful: %i: %i\n",i,*ptr);
		}
	}	
	
}

int	weSplashInit(char *path_res, float *projection)
{
	float white_color[3]={1.0,1.0,1.0};
	char splash_fragment[256]="splash_fragment";
	char splash_vertex[256]="splash_vertex";
	char buffer[256];
	int res;
	int i;
	float modelview[20];
	float *ptr_aux=NULL;
	float norm;

	printf("Loading resources for splash screen\n");
	

	if(path_res==NULL)
	{
		printf("Warning!!!, no path was specified for resources, trying with the local folder\n");
	}
	else
	{
		sprintf(splash_fragment,"%s/splash_fragment");
		sprintf(splash_vertex,"%s/splash_vertex");
	}

	printf("Loading shaders...\n");

	res=glsutLoadShaders(stdout,&splash_shaders,splash_fragment,splash_vertex);

	if(res<0)
	{
		return 0;
	}

	glUseProgram(splash_shaders.program);
	queryVars(); //query for the vars in the shader

	printf("Resource shaders loaded succesfully, continue with the textures\n");
	
	//glGenTextures(13,&textures_id);

	for(i=0;i<13;i++)
	{
		if(path_res==NULL)
			sprintf(buffer,"%s",textureNames[i]);
		else
			sprintf(buffer,"%s/%s",path_res,textureNames[i]);
		printf("%s \n",buffer);

		textures_id[i]=SOIL_load_OGL_texture(buffer,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,
				      SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

		glBindTexture(GL_TEXTURE_2D,textures_id[i]);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

		weQuadInit(WireArray+i,white_color,textures_id[i]);
	}

	glUniformMatrix4fv(splashVars.Projection,1,0,projection);

	shader_vars.vertex_attrib	=splashVars.Vertex;
	shader_vars.color_attrib	=splashVars.Colors;
	shader_vars.texture_attrib	=splashVars.Texture;
	shader_vars.sampler		=splashVars.Sampler;
	shader_vars.modelview		=splashVars.Modelview;

	weCatmullArrayCreate(&anim_position,10);
	ptr_aux=(float*)anim_position.points;

	for(i=0;i<anim_position.nPoints;i++,ptr_aux+=3)
	{
		norm=((i/(float)anim_position.nPoints) - 0.5)*2;
		ptr_aux[0]=norm;
		ptr_aux[1]=fabs(0.5*exp(-norm)*sin(5*norm));
		ptr_aux[2]=0;
	}

	weCatmullArrayCreate(&anim_scale,10);
	ptr_aux=(float*)anim_scale.points;

	for(i=0;i<anim_scale.nPoints;i++,ptr_aux+=3)
	{
		norm=((i/(float)anim_scale.nPoints) - 0.5)*2;
		ptr_aux[0]=fabs(cos(5*norm)*0.5)+0.5;
		ptr_aux[1]=fabs(sin(5*norm))+0.5;
		ptr_aux[2]=0;
	}
	
	ptr_aux=(float*)anim_position.points;
	printf("Printing points...%i\n",anim_position.nPoints);
	for(i=0;i<anim_position.nPoints;i++,ptr_aux+=3)
	{
		printf("%f, %f, %f\n",ptr_aux[0],ptr_aux[1],ptr_aux[2]);
	}

	quad_anim.quad=WireArray;
	quad_anim.position=&anim_position;
	quad_anim.scale=&anim_scale;

	ready=1;

	return 0;
}

int	weSplashPlay()
{
	int i;
	float mv[16];
	static float s=0.0f;
	float black[3]={0.0,0.0,0.0};	
	int res;

	if(ready==0)
	{
		printf("Sorry, you need call weSplashInit() function before calling this\n");
		return -1;
	}

	glUseProgram(splash_shaders.program);


	/*
	wematIdentity(mv);
	//wematRotate(s,s,mv);
	wematAddScale(0.5,0.5,0.5,mv);
	wematAddTranslate(-0.75,0.0,0.0,mv);  

	s+=0.01;

	for(i=0;i<4;i++)
	{
		//weDrawQuad(WireArray+i,mv);
		weQuadDraw(WireArray+i,&shader_vars,mv);
		wematAddTranslate(0.5,0.0,0.0,mv);
	}
	*/	

	/*
	wematIdentity(mv);
	res=weVect3Draw(anim_position.points,anim_position.nPoints,&shader_vars,black,mv);

	if(res<0)
		printf("Error: %i\n",res);
	*/

	wematIdentity(mv);
	//weQuadDraw(WireArray,&shader_vars,mv);
	weCatmullQuadAnimDisplay(&quad_anim,&shader_vars,0.1,mv);


	return 0;   
}

int 	weSplashDeallocate()
{
	
}