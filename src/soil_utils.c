#include <soil_utils.h>

static int image_counter=0;
static unsigned char imageData[1080*2000*3];

/**
       * Grab and save the image from the framebuffer
       * @param filename a string containing the full filename
       * @return 0 if all are correct, -1 if we have and error
*/
int suGrabAndSave(char *filename)
{
	int dimentions[4];
	int save_result;
	glGetIntegerv(GL_VIEWPORT,dimentions);

	printf("Width: %i, Height: %i\n",dimentions[2],dimentions[3]);

	glReadPixels(0,0,dimentions[2],dimentions[3],GL_RGB,GL_UNSIGNED_BYTE,imageData);

	save_result=SOIL_save_image(filename,SOIL_SAVE_TYPE_DDS,dimentions[2],dimentions[3],3,imageData);

	if(save_result<0)
		return -1;

	return 0;
}


/**
       * Set the basename for a continuos image writing is desired.
       * @param name_base a string contaigint the base name
       * @return 0 if all are correct, -1 if we have and error
*/
int suSetBaseName(char *name_base)
{
	return 0;
}

/**
       * Grab the image from Framebuffer and write the image (base+serial)
       * @return 0 if all are correct, -1 if we have and error
*/
int suGrabBase()
{
	return 0;
}

/**
       * Reset the internal count used in suSetBaseName
       * @param init_value the init value of the counter (int)
       * @see suSetBaseName
       * @see suGrabBase
       * @return 0 if all are correct, -1 if we have and error
*/
int suSetBaseCounter(int init_value)
{
	return 0;
}


