//gcc -o fft_capture.bin fft_capture.c -lasound -lfftw3 -lm
//./fft_capture.bin "plughw:1,0"
#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <fftw3.h>
#include <math.h>

#define FFT_NUM_PT 1024
	      
main (int argc, char *argv[])
{
		int i,j;
		int err;
		short buf[65440];
		snd_pcm_t *capture_handle;
		snd_pcm_hw_params_t *hw_params;
		int rate,newRate;
		//FFTW vars
		fftw_complex *in,*out;
		fftw_plan p;
		double abs_band;

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
	
		/*
		printf("Recording...\n");
		for (i = 0; i < 200; ++i) {
			
			if ((err = snd_pcm_readi (capture_handle, buf, 1024)) != 1024) {
				fprintf (stderr, "read from audio interface failed (%s)\n",
					 snd_strerror (err));
				exit (1);
			}

		}*/

		printf("End of recording\n");

		//Let's begin with FFTW
		in=(fftw_complex*)fftw_malloc(sizeof(fftw_complex)*FFT_NUM_PT);
		out=(fftw_complex*)fftw_malloc(sizeof(fftw_complex)*FFT_NUM_PT);
		p=fftw_plan_dft_1d(FFT_NUM_PT,in,out,FFTW_FORWARD,FFTW_ESTIMATE);

		//let's make some transforms

		printf("Recording...\n");
                for (i = 0; i < 2000; ++i) {

                        if ((err = snd_pcm_readi (capture_handle, buf, 1024)) != 1024) {
                                fprintf (stderr, "read from audio interface failed (%s)\n",
                                         snd_strerror (err));
                                exit (1);
                        }

			//let's copy the data into the FFTW, from S16 to Double
			for(j=0;j<FFT_NUM_PT;j++)
			{
				in[j][0]=buf[j]; //Real part
				in[j][1]=0;      //Complex part
			}

			//Transform it!
			fftw_execute(p);

			//print the first bands (low freq)
			for(j=4;j<5;j++)
			{
				abs_band=sqrt(out[j][0]*out[j][0] + out[j][1]*out[j][1]);
				printf("%lf \t\t ",abs_band);
			}

			for(j=100;j<101;j++)
                        {
                                abs_band=sqrt(out[j][0]*out[j][0] + out[j][1]*out[j][1]);
                                printf("%lf \t\t ",abs_band);
                        }

			for(j=300;j<301;j++)
                        {
                                abs_band=sqrt(out[j][0]*out[j][0] + out[j][1]*out[j][1]);
                                printf("%lf \t\t ",abs_band);
                        }



			printf("\n");

                }

		fftw_destroy_plan(p);
		fftw_free(in); fftw_free(out);
		snd_pcm_close (capture_handle);
		exit (0);
}

