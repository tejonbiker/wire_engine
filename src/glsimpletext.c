#include "glsimpletext.h"

int glstextBuildTextures(GLint *texture_array)
{
	int i;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(N_CTEXT, (unsigned int*)texture_array);
	for(i=0;i<N_CTEXT;i++)
	{
		glBindTexture(GL_TEXTURE_2D,texture_array[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 8, 8, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,arraycTextures[i]);
		//printf("%i,Error: %i\n",  PatternData[0].centroidText[i],glGetError());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	return 0;
}
