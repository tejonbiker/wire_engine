#include "wecatmull.h"

int weCatmullArrayCreate(CatmullArray *array,int nPoints)				//Create memory ofr array of "nPoints"
{
	if(array==NULL || nPoints<=0)
		return -1;

	array->points=(WEVect3*)malloc(sizeof(WEVect3)*nPoints);

	if(array->points==NULL)
		return -2;

	array->nPoints=nPoints;
	array->currentPoint=0;
	array->currentEval=0.0f;

	return 0;
}

int weCatmullArrayDelete(CatmullArray *array)						//Delete Arrays
{
	free(array->points);

	return 0;
}

int weCatmullEval(float *p1,float *p2, float *p3, float *p4,float t, float *result)	//Evaluate 4 points through "t"
{

	int i;

	for(i=0;i<3;i++)
	{
		result[0]= 0.5*(( -p1[0] + 3*p2[0] - 3*p3[0] + p4[0])*t*t*t
		  		+(2*p1[0] -5*p2[0] + 4*p3[0] - p4[0])*t*t
				+(-p1[0]+p3[0])*t
				+2*p2[0] );
		result++; p1++; p2++; p3++; p4++;
	}

	return 0;
}

int weCatmullEvalArray(CatmullArray *pointList,float step, float *result_cat)				//Evaluate an Array of points, automatically change segments 
{

	int basePoint;
	float *currentEval=NULL;
	int end_run=0;

	if(pointList==NULL)
		return -1;

	if(pointList->nPoints<4)
		return -2;

	currentEval=&(pointList->currentEval);

	*currentEval+=step;

	if(*currentEval>=1.0f)
	{
		*currentEval=1.0f;
		if(pointList->currentPoint+4<pointList->nPoints)
		{
			*currentEval=0.0f;
			pointList->currentPoint++;
			
		}else
		{
			end_run=1;
		}
		
	}
	
	basePoint=pointList->currentPoint;

	weCatmullEval(&(pointList->points[basePoint].x),
		  &(pointList->points[basePoint+1].x),
		  &(pointList->points[basePoint+2].x),
		  &(pointList->points[basePoint+3].x),*currentEval,result_cat); 

	if(end_run==1)
		return 1;
	return 0;
}
 
int weCatmullArrayFromFile(char *filename,CatmullArray *arrayDest)			//Load a list of points from file
{
	arrayDest->currentPoint=0;
	arrayDest->currentEval=0.0f;
	return weVect3FromFile(filename,&(arrayDest->points),&(arrayDest->nPoints));
}

int weCatmullRewind(CatmullArray *array)
{
	if(array==NULL)
		return -1;

	array->currentPoint=0;
	array->currentEval=0.0f;
}

int weCatmullQuadAnimDisplay(WEQuadAnim *quad_anim,WEShaderVars *shader_vars,float step, float *mv)
{
	WEQuad 		*quad=NULL;
	CatmullArray 	*position=NULL,*scale=NULL;
	float		cat_pos[3], cat_scale[3];
	int		res_pos=0,res_scale=0;
	float		mv_quad[16];

	if(quad_anim==NULL || shader_vars==NULL)
		return -1;

	quad 	 =(quad_anim->quad);
	position =(quad_anim->position);
	scale 	 =(quad_anim->scale);

	if(quad==NULL && (position==NULL || scale==NULL) )
		return -2;

	memcpy(mv_quad,mv,sizeof(float)*16);

	if(scale!=NULL)
	{
		res_scale=weCatmullEvalArray(scale,step,cat_scale);
		wematAddScale(cat_scale[0],cat_scale[1],cat_scale[2],mv_quad);
	}

	if(position!=NULL);
	{
		res_pos=weCatmullEvalArray(position,step,cat_pos);
		wematAddTranslate(cat_pos[0],cat_pos[1],cat_pos[2],mv_quad);
	}


	weQuadDraw(quad,shader_vars,mv_quad);

	return 0;
}
