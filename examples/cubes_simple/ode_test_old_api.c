#define dSINGLE

#include <stdio.h>
#include "glpi.h"
	#include "matrix.h"
	#include "glplot.h"
#include <ode/ode.h>



typedef struct  ODECubeTag{
	dBodyID body;
	dGeomID geom;
	PlotBasicAsset *cube;
	float l;
}ODECube;

#define N_CUBES 10
#define MAX_CONTACTS 8
ODECube CubeTest[N_CUBES];

static dWorldID world;
static dSpaceID space;
static dGeomID ground;
static dJointGroupID contactgroup;


static void nearCallback(void *data, dGeomID o1, dGeomID o2)
{
    int i;
    // if (o1->body && o2->body) return;
	
    // exit without doing anything if the two bodies are connected by a joint
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);
    if (b1 && b2 && dAreConnectedExcluding(b1, b2, dJointTypeContact))
        return;
	
    dContact contact[MAX_CONTACTS];	// up to MAX_CONTACTS contacts per box-box
    for (i = 0; i < MAX_CONTACTS; i++) {
        contact[i].surface.mode = dContactBounce | dContactSoftCFM;
        contact[i].surface.mu = dInfinity;
        contact[i].surface.mu2 = 0;
        contact[i].surface.bounce = 0.2;
        contact[i].surface.bounce_vel = 0.2;
        contact[i].surface.soft_cfm = 0.01;
    }
    int numc = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom,
                        sizeof(dContact));
    if (numc) {
        dMatrix3 RI;
        dRSetIdentity(RI);
        for (i = 0; i < numc; i++) {
            dJointID c =
			dJointCreateContact(world, contactgroup, contact + i);
            dJointAttach(c, b1, b2);
        }
    }
	
}

 
 
void display()
{
	int i,j;
	float *mult[16];
	static float theta=0.0f;
	static float trans[16];
	static float rot[16];
	int idText;
	int idNexText;
	const dReal *pos,*R;
	const dReal *vel;
	float GlobalVel=0;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	dSpaceCollide(space, 0, &nearCallback);
	//dWorldStep(world,0.01);
	dWorldQuickStep(world, 1. / 50);
	dJointGroupEmpty(contactgroup);
	
	for(i=0;i<N_CUBES;i++)
	{
		pos = dBodyGetPosition(CubeTest[i].body);
		R  = dBodyGetRotation(CubeTest[i].body);
		vel = dBodyGetLinearVel(CubeTest[i].body);
		
		GlobalVel=vel[0]*vel[0]+vel[1]*vel[1]+vel[2]*vel[2];
	
	
		matrixODEtoGL((float*)R,(float*)pos, CubeTest[i].cube->matrix_modelview);
		matrixAddTranslate(-CubeTest[i].l/2.0,-CubeTest[i].l/2.0,CubeTest[i].l/2.0,CubeTest[i].cube->matrix_modelview);
	
		//printf("%f, %f, %f\n",pos[0],pos[1],pos[2]);
		
		glBindTexture(GL_TEXTURE_2D, glplotGetIDTexture(2));
		glplotBasicAsset(CubeTest[i].cube);
		
	}

	if(GlobalVel<0.000001f)
	{
		for(i=0;i<N_CUBES;i++)
			dBodySetPosition(CubeTest[i].body,-1.0+ 2.0*(-rand()%1024+1024)/1024.0f, 0.5 + (rand()%1024)/2048.0f, 0);
	}
	
	
	glpiSwapBuffers();
}



void setup()
{

	
	int epocs;
	int i,j;
	Matrix *Circles=NULL;
	char nameBuffer[256];
	PlotShaderInfo SInfo;
	dMass m1;
	
	const dReal radius = 0.1;
	const dReal mass=1.0;
	
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glClearDepthf(1.0f);
	
	glplotInit();
	
	srand(time(NULL));
	
	dInitODE();
	world = dWorldCreate();
	space = dHashSpaceCreate(0);
	contactgroup = dJointGroupCreate(0);
	
	dWorldSetGravity(world, 0, -0.1, 0.0);
	dWorldSetCFM(world, 1e-5);
	
	ground = dCreatePlane(space, 0, 1, 0, -0.5);
	
	
	for(i=0;i<N_CUBES;i++)
	{
		CubeTest[i].body = dBodyCreate(world);
		dMassSetZero(&m1);
		dMassSetBox(&m1, mass, radius, radius, radius);
		dBodySetMass(CubeTest[i].body,&m1);
		dBodySetPosition(CubeTest[i].body,-1.0+ 2.0*(-rand()%1024+1024)/1024.0f, 0.5 + (rand()%1024)/2048.0f, 0);
	
		CubeTest[i].geom = dCreateBox(space, radius, radius, radius);
		dGeomSetBody(CubeTest[i].geom, CubeTest[i].body);
	
		glplotBasicAssetCube(&(CubeTest[i].cube), radius, NULL);
	
		CubeTest[i].l=radius;
	}
	
}


int main (void) {

int res=0;

	res =glpiInit();

	printf("\nVerificando Resultado\n");

	if(res<0)
	{
		printf("Error: %i",res);
		return 0;
	}

	
	glplotInit();
	setup();

	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glClearColor(1.0f,1.0f,1.0f,1.0f);

	while(1){
		display();
	}	

	glplotEnd();
	glpiEnd();
	return 0;
	
    return 0;
}
