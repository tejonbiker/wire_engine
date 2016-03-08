#include <stdio.h>
#include "glpi.h"
#include "wire_engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <fftw3.h>
#include <math.h>

/*
Simple example that shows a white_screen, the interesting part of this example resides in 
glpi.h and glpi.c, this files are that init the buffers in the raspberry pi, this way of work
don't use X window, draw direct in the screen.
*/

int finish=0;
#define FFT_NUM_PT 1024
int err;
short buf[65440];
snd_pcm_t *capture_handle;
snd_pcm_hw_params_t *hw_params;
int rate,newRate;
//FFTW vars
fftw_complex *in,*out;
fftw_plan p;
double abs_band;

int win_width, win_height;
float proj_matrix[16];
float globalmv[16];
BaseShader 	ShaderInfo;
WEShaderVars   points_shader_vars;
WEQuad fft_bar[512];



void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	
	wematIdentity(globalmv);
	weQuadDraw(fft_bar,&points_shader_vars,globalmv);
		
	glFlush();
	glFinish();	
	glpiSwapBuffers();
}

void setup()
{
	
}

void init_alsa(int argc, char *argv[])
{
		if ((err = snd_pcm_open (&capture_handle, argv[1], SND_PCM_STREAM_CAPTURE, 0)) < 0) {
			fprintf (stderr, "cannot open audio device %s (%s)\n", 
				 argv[1],
				 snd_strerror (err));
			exit (1);
		}
		   
		if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
			fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
				 snd_strerror (err));
			exit (1);
		}

				 
		if ((err = snd_pcm_hw_params_any (capture_handle, hw_params)) < 0) {
			fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
	
		if ((err = snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
			fprintf (stderr, "cannot set access type (%s)\n",
				 snd_strerror (err));
			exit (1);
		}

	
		if ((err = snd_pcm_hw_params_set_format (capture_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
			fprintf (stderr, "cannot set sample format (%s)\n",
				 snd_strerror (err));
			exit (1);
		}

		rate=newRate = 44100;
		if ((err = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params,&newRate, 0)) < 0) {
			fprintf (stderr, "cannot set sample rate (%s)\n",
				 snd_strerror (err));
			exit (1);
		}

		if(newRate!=rate)
			printf("Requested sample rate %i Hz, but got %i Hz\n",rate,newRate);
	
		if ((err = snd_pcm_hw_params_set_channels (capture_handle, hw_params, 2)) < 0) {
			fprintf (stderr, "cannot set channel count (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
	
		if ((err = snd_pcm_hw_params (capture_handle, hw_params)) < 0) {
			fprintf (stderr, "cannot set parameters (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
	
		snd_pcm_hw_params_free (hw_params);
	
		if ((err = snd_pcm_prepare (capture_handle)) < 0) {
			fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
				 snd_strerror (err));
			exit (1);
		}

		printf("End of recording\n");

		//Let's begin with FFTW
	    in=(fftw_complex*)fftw_malloc(sizeof(fftw_complex)*FFT_NUM_PT);
		out=(fftw_complex*)fftw_malloc(sizeof(fftw_complex)*FFT_NUM_PT);
		p=fftw_plan_dft_1d(FFT_NUM_PT,in,out,FFTW_FORWARD,FFTW_ESTIMATE);
}

void window_resize()
{
	float aspect= win_width/(float)win_height;

	glViewport(0,0,win_width,win_height);

	if(aspect>=1.0f)
		wematOrtho(-1*aspect,1*aspect,-1,1,-1,1,proj_matrix);
	else
		wematOrtho(-1,1,-1/aspect,1/aspect,-1,1,proj_matrix);

	printf("Projection matrix result...\n");
	wematPrint(proj_matrix);

	//glUniformMatrix4fv(points_shader_vars.projection,1,0,proj_matrix);

}

void 	getShaderVars()
{
	int i;
	GLint *ptrVars=NULL;

	memset(&points_shader_vars,0,sizeof(WEShaderVars));

	points_shader_vars.projection	  =glGetUniformLocation(ShaderInfo.program,"Projection");
	points_shader_vars.modelview      =glGetUniformLocation(ShaderInfo.program,"Modelview");
	points_shader_vars.vertex_attrib  =glGetAttribLocation(ShaderInfo.program,"Position");
	points_shader_vars.color_attrib   =glGetAttribLocation(ShaderInfo.program,"Colors");	
	points_shader_vars.normal_attrib  =glGetAttribLocation(ShaderInfo.program,"Normal");
	points_shader_vars.normal_matrix  =glGetUniformLocation(ShaderInfo.program,"NormalMatrix");
	points_shader_vars.program_id     =ShaderInfo.program;
	
	printf("Printing Querying Vars...\n");
	weShaderVarsPrint(&points_shader_vars);

	/*	
	ptrVars=(GLint*)(&VarShaders);
	for(i=0;i<sizeof(varShaders);i+=sizeof(int))
	{
		printf("%i\n",*ptrVars);
		if(*ptrVars<0)
		{
			printf("Error to get Vars from shader\n");
			exit(0);
		}
		ptrVars++;
	}
	*/
}

void setup_gles()
{
	float testm[16];
	int i;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepthf(1.0f);
	glDepthMask(GL_TRUE);
	//glDepthRange(0.0f, 1.0f);
	float color_bar[3]={0.5,0.5,0.5};

	printf("Loading shaders....\n");

	if(glsutLoadShaders(stdout,&ShaderInfo,"fragment_shader","vertex_shader")<0)
	{
		exit(0);
	}

	printf("Shaders loaded succesfully\n");

	getShaderVars();

	srand(time(NULL));

	glpiGetWindowSize(&win_width,&win_height);

	window_resize();

	printf("%i x %i \n",win_width,win_height);
	
	for(i=0;i<512;i++)
		weQuadInit(fft_bar+i,color_bar,0);
}

int main(int argc, char *argv[])
{
		
	
	int res=0;

	res =glpiInit();

	if(res<0)
	{
		printf("Error: %i",res);
		return 0;
	}

	setup();
	init_alsa(argc,argv); 	
	setup_gles();

	glBindFramebuffer(GL_FRAMEBUFFER,0);
	
	//the only activity in this example is chagne the color of the screen
	glClearColor(1.0f,1.0f,1.0f,1.0f); 	

	while(!finish){
		display();
	}	

	glpiEnd();
	return 0;
}

