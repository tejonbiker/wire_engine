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
		
		fftw_destroy_plan(p);
		fftw_free(in); fftw_free(out);
		snd_pcm_close (capture_handle);
}

int main(int argc, char *argv[])
{
		
	init_alsa(argc,argv); 	

}

