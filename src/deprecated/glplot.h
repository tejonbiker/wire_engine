/*
 *  glplot.h
 *  HCM
 *
 *  Created by José Federico  Ramos Ortega on 17/02/13.
 *  Copyright 2013 Facultad De Ingeniería Electronica. All rights reserved.
 *
 */

#ifndef _GLPLOT_
#define _GLPLOT_

#ifdef  GLPI
	#include "GLES2/gl2.h"
	#include <stdbool.h>

	#warning "GLES Pi Headers in use"
#else
	#warning "Mac Headers not used"
#endif

#ifdef __APPLE__
	#include <OpenGL/OpenGL.h>
	#warning "Mac OS OpenGL Headers in use"
#else
	#warning "Pi headers not used"
#endif

#ifdef _WIN32


    #define GLEW_STATIC
    #include <windows.h>
    #include <GL/glew.h>
    #include <GL/glut.h>
    #include<GL/gl.h>
    #include<GL/glu.h>
    #include<GL/glext.h>

    //typedef char  GLchar;
    #warning "Windows 32 bits detected"
#endif

#ifdef _WIN64
    #include<GL\gl.h>
    #include<GL\glu.h>
    #include<GL\glext.h>
    typedef char  GLchar;
    #warning "Windows 32 bits detected"
#endif


#include <stdio.h>
#include <string.h>
#include "centroidtextures.h"
#include "matrix.h"

#define glplotScatterColor(A, B, C)  glplotColor(A,B,C)
#define GLPL_ENABLE   1
#define GLPL_DISABLE  0

#define GLPL_MAX_SHADER_SIZE 5000

/*
 Simple API to plot graphics on OpenGL
*/

enum{
	PLOT_ERR_ARG=-1,
	PLOT_ERR_MEM,
	PLOT_ERR_DIM,
	PLOT_ERR_FILE,
};

enum{
	PLOT_ASSET_CUBE=1000,
};

typedef struct plotStatsTag{
	float xMin,xMax;
	float yMin,yMax;
	float Modelview[16];
	float Projection[16];
	int   idModelview;
	int   idNormalMat;
	int   idProjection;
	int   idAttribPosition;
	int	  idAttribColor;
	int	  idAttribTexture;
	int	  idAttribNormal;
	int   idTexture;
	int	  idplotPoints;
	int	  programHandle;
	float *points;
	float *colors;
	int   nPoints;
	int		vertexShader;
	int		fragmentShader;

}PlotStats;

typedef struct plotPatternDataTag{
	int    centroidText[15];
	int	   whiteTexture;
	float  centroidColor[15*3];
	int	   plotPoints;
}PlotPatternData;

typedef struct PlotShaderTag{
	GLint program;
	GLint vertex_shader;
	GLint fragment_shader;
}PlotShaderInfo;

//Related Structures for GLSL and lighting
typedef struct loc_DirectionalLightTag{
	GLint direction;
	GLint halfplane;
	GLint ambientColor;
	GLint diffuseColor;
	GLint specularColor;
}loc_DirectionalLight;

typedef struct MaterialTag{
	GLint ambientFactor;
	GLint diffuseFactor;
	GLint specularFactor;
	GLint shininess;
}loc_Material;

typedef struct u_DirectionalLightLocTag{
	float direction[3];
	float halfplane[3];
	float ambientColor[4];
	float diffuseColor[4];
	float specularColor[4];
}u_DirectionalLightLoc;

typedef struct u_MaterialTag{
	float ambientFactor[4];
	float diffuseFactor[4];
	float specularFactor[4];
	float shininess;
}u_Material;

typedef struct PlotBasicAssetTag{
	int		nVertices;
	float	*vertices;
	int		nColors;
	float	*colorsRGB;
	int		nTexcoord;
	float	*textcoord2D;
	int		nindex;
	GLushort  *index;
	int		nNormals;
	float   *normals;
	float	matrix_modelview[16];

	u_Material assetMaterial;
}PlotBasicAsset;

//matrix utility functions for 4x4 matrix float type
void	matrixPrint(float *A);
void	matrixTranslateScale(float x,float y, float z, float s, float *m);
void    matrixAddScale(float sx,float sy,float sz, float *m);
void	matrixAddTranslate(float x,float y, float z, float *m);
int		matrixRotate(float polar,float azimutal, float *m);
void	matrixIdentity(float *m);
void	matrixMult(float *A, float *B, float *C);
int		matrixInverse(float *A, float *result);
int		matrixODEtoGL(float *rot,float *pos,float *m16);
int		matrixTransponse(float *A, float *result);
int		matrixOrtho(float left, float right, float bottom, float top, float nearVal, float farVal, float *m);
int     matrixMultVector(float *m, float *vec4, float *res4);



int		glplotInit();
int		glplotGetIDTexture(int indexTexture);
int		glplotGetPlotStats(PlotStats **status);
void	glplotSetProjectionMatrix(float *m4);
int		glplotEnd();
void	glplotQuickTest(); //for debuggin purposes
int		glplotScatterColor(float r, float g, float b);
int		glplotAutoset(int enable);
int		glplotHCM2D(Matrix *Samples, Matrix *U, Matrix *Centroids);//not use
int		glplotHCMN(Matrix *Samples, Matrix *U, Matrix *Centroids);
int		glplotScatter2D(Matrix *Samples);//Not use, instead use 3D mod
int		glplotScatter3D(Matrix *samples);
int		glplotLineStrip(Matrix  *Samples);//Plot Line Stip with Samples as Matrix with  Dim X NSamples dimentions
int		glplotTestTexture();
int		glplotApplyRotations(float polar,float azimutal);//Azimutal it's along the ecuator (x to -x), polar it's between poles(y to -y)

int		glplotLoadShaders(FILE *messages,PlotShaderInfo *info_shader, char *fragment_file, char *vertex_file); //Function to load shaders, messages can be a pointer to
																											//std out or file out.
int		glplotLoadShadersChar(FILE *messages,PlotShaderInfo *info_shader,const  char *fragment_src, const char *vertex_src);

int		glplotBasicAssetCreate(PlotBasicAsset **asset, int nvertices, int ncolors, int ntexcoord, int nnormals ,int nindex);
int		glplotBasicAssetDetelete(PlotBasicAsset **asset);
int		glplotBasicAssetCube(PlotBasicAsset **asset,float edge,float *color_v);
int		glplotBasicAssetPlane(PlotBasicAsset **asset,float *pi, float *pf,int segments);
int		glplotBasicAsset(PlotBasicAsset *asset);

#endif

