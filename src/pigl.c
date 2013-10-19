#include "pigl.h"

ScreenData ScreenState;

char  __keys[256];
int __mouse[3];
char __rel_mouse;

int __key_fd=0; // defaults to 0 ie console

int glpiInit()
{
	int			sucess;
	EGLBoolean	result;
	EGLint		num_config;
	
	static EGL_DISPMANX_WINDOW_T window;
	
	DISPMANX_ELMENT_HANDLE_T	DispElement;
	DISPMANX_DISPLAY_HANDLE_T	DispDisplay;
	DISPMANX_UPDATE_HANDLE_T    DispUpdate;
	VC_RECT_T dst_rect;
	VC_RECT_T src_rect;
	
	static const EGLint attributes[]={
		EGL_RED_SIZE,8,
		EGL_GREEN_SIZE,8,
		EGL_BLUE_SIZE,8,
		EGL_ALPHA_SIZE,8,
		EGL_DEPTH_SIZE, 16,lkl.-.-a.-.a
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_NONE
	};
	
	EGLConfig config;
	
	ScreenState.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if(ScreenState.display == EGL_NO_DISPLAY)
		return -1;
	
	result = eglInitialize(state->display,NULL,NULL);
	if(result == EGL_FALSE)
		return -2;
	
	result = eglChooseConfig(ScreenState.display, attributes,&config,1,&num_config);
	if(result == EGL_FALSE)
		return -3;
	
	ScreenState.context = eglCreateContext(ScreenState->display,config,EGL_NO_CONTEXT,NULL);
	if(ScreenState.context == EGL_NO_CONTEXT)
		return -4;
	
	sucess = graphics_get_display_size(0,&ScreenState.width,&ScreenState.height);
	if(sucess<0)
		return -5;
	
	dst_rect.x=0;
	dst_rect.y=0;
	dst_rect.width = ScreenState.width;
	dst_rect.height= ScreenState.height;
	
	src_rect.x=0;
	src_rect.y=0;
	src_rect.width = ScreenState.width<<16;
	src_rect.height= ScreenState.height<<16;
	
	DispDisplay = vc_dispmanx_display_open(0);
	DispUpdate  = vc_dispmanx_update_start(0);
 	
	DispElement= vc_dispmanx_display_add(DispUpdate,DispDisplay,0,&dst_rect,0,&src_rect,DISMANX_PROTECTION_NONE,0,0,0);
	
	window.element=DispElement;
	window.width=ScreenState.width;
	window.height=ScreenState.height;
	vc_dispmanx_update_submit_sync(DispUpdate);
	
	ScreenState.surface = eglCreateWindowSurface(ScreenState.display, config, &window, NULL);
	if(ScreenState.surface == EGL_NO_SURFACE )
		return -6;
	
	result = eglMakeCurrent(ScreenState.display, ScreenState.surface,ScreenState.context);
	if(result==EGL_FALSE)
		return -7;
}

int glpiEnd()
{
	glClear(GL_COLOR_BUFFER_BIT);
	eglSwapBuffers(ScreenState.display, ScreenState.surface);
	
	eglMakeCurrent(ScreenState.display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);
	eglDestroyContext(ScreenState.display,ScreenState.context);
	eglDestroySurface(ScreenState.display,ScreenState.surface);
	eglTerminate(ScreenState.display);
}
