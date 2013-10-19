#include "glpi.h"

ScreenData ScreenState;

char __keys[256];
int __mouse[3];
char __rel_mouse;

int __key_fd=0; // defaults to 0 ie console

int __display_width,__display_height;

int glpiInit()
{
	int			sucess;
	EGLBoolean	result;
	EGLint		num_config;

	static EGL_DISPMANX_WINDOW_T window;

	DISPMANX_ELEMENT_HANDLE_T	DispElement;
	DISPMANX_DISPLAY_HANDLE_T	DispDisplay;
	DISPMANX_UPDATE_HANDLE_T    DispUpdate;
	VC_RECT_T dst_rect;
	VC_RECT_T src_rect;

	static const EGLint attributes[]={
		EGL_RED_SIZE,8,
		EGL_GREEN_SIZE,8,
		EGL_BLUE_SIZE,8,
		EGL_ALPHA_SIZE,8,
		EGL_DEPTH_SIZE, 16,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_NONE
	};

	static const EGLint context_attributes[] =
   	{
      		EGL_CONTEXT_CLIENT_VERSION, 2,
      		EGL_NONE
   	};

	EGLConfig config;

	bcm_host_init();

	memset(&(ScreenState),0,sizeof(ScreenData));

	ScreenState.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if(ScreenState.display == EGL_NO_DISPLAY)
		return -1;

	result = eglInitialize(ScreenState.display,NULL,NULL);
	if(result == EGL_FALSE)
		return -2;



	result = eglChooseConfig(ScreenState.display, attributes,&config,1,&num_config);
	if(result == EGL_FALSE)
		return -3;

	result = eglBindAPI(EGL_OPENGL_ES_API);
         if(result == EGL_FALSE)
                return -10;


	ScreenState.context = eglCreateContext(ScreenState.display,config,EGL_NO_CONTEXT,context_attributes);
	if(ScreenState.context == EGL_NO_CONTEXT)
		return -4;



	sucess = graphics_get_display_size(0,&(ScreenState.width),&(ScreenState.height));
	if(sucess<0)
		return -5;

	dst_rect.x=0;
	dst_rect.y=0;
	dst_rect.width = ScreenState.width;
	dst_rect.height= ScreenState.height;

	__display_width=ScreenState.width;
	__display_height=ScreenState.height;

	src_rect.x=0;
	src_rect.y=0;
	src_rect.width = ScreenState.width<<16;
	src_rect.height= ScreenState.height<<16;


	DispDisplay = vc_dispmanx_display_open(0);
	DispUpdate  = vc_dispmanx_update_start(0);

	DispElement= vc_dispmanx_element_add(DispUpdate,DispDisplay,0,&dst_rect,0,&src_rect,DISPMANX_PROTECTION_NONE,0,0,0);

	window.element=DispElement;
	window.width=ScreenState.width;
	window.height=ScreenState.height;
	vc_dispmanx_update_submit_sync(DispUpdate);

	ScreenState.surface = eglCreateWindowSurface(ScreenState.display, config, &window, NULL);
	if(ScreenState.surface == EGL_NO_SURFACE )
		return -6;

	result = eglMakeCurrent(ScreenState.display, ScreenState.surface, ScreenState.surface,ScreenState.context);
	if(result==EGL_FALSE)
		return -7;


   	glViewport(0, 0, (GLsizei)ScreenState.width,(GLsizei)ScreenState.height);

	return 0;
}

int glpiSwapBuffers()
{
	eglSwapBuffers(ScreenState.display, ScreenState.surface);
	return 0;
}

int glpiEnd()
{
	glClear(GL_COLOR_BUFFER_BIT);
	eglSwapBuffers(ScreenState.display, ScreenState.surface);

	eglMakeCurrent(ScreenState.display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);
	eglDestroyContext(ScreenState.display,ScreenState.context);
	eglDestroySurface(ScreenState.display,ScreenState.surface);
	eglTerminate(ScreenState.display);

	return 0;
}

void	glpiDoEvents()
{
	char buf[1];
	int res;
	struct input_event ev;


	if(__key_fd==0)
	{
		res=read(__key_fd,&buf[0],1);
		while(res>=0)
		{
			if(buf[0]&0x80) //Key released
			{
				__keys[buf[0]&~0x80]=0;
			}else
			{
				__keys[buf[0]&~0x80]=1;
				//dflag(buf[0]&~0x80d);
			}

			res=read(__key_fd,&buf[0],1);
		}
	}else
	{
		res=read(__key_fd,&ev,sizeof(struct input_event));
		while(res>=0)
		{
			if(ev.type=EV_KEY)
			{
				if(ev.value==1)
				{
					__keys[ev.code & 0xFF]=1;
				}else if(ev.value==0)
				{
					__keys[ev.code & 0xFF]=0;
				}
			}
		}
	}

	if(__rel_mouse)
	{
		__mouse[0]=0;
		__mouse[1]=0;
	}

	if(__mouse_fd>0)
	{
		signed char mbuf[3];
		int mres;
		mres=read(__mouse_fd,&mbuf[0],3);
		while(mres>=0)
		{
			__mouse[2]=mbuf[0]&7;
			if(__rel_mouse)
			{
				__mouse[0]=mbuf[1];
				__mouse[1]=-mbuf[2];
			}else
			{
				__mouse[0]=__mouse[0]+mbuf[1];
				__mouse[1]=__mouse[1]-mbuf[2];
				if(__mouse[0]<0)
					__mouse[0]=0;
				if(__mouse[1]<0)
					__mouse[1]=0;
				if(__mouse[0]>__display_width)
					__mouse[0]=__display_width;
				if(__mouse[1]>__display_height)
					__mouse[1]=__display_height;
			}

			mres=read(__mouse_fd,&mbuf[0],3);
		}
	}
}

int*	glpiGetMouseArray()
{
	__rel_mouse=0;

	__mouse_fd=open("/dev/input/mouse0",O_RDONLY);
	if(__mouse_fd<0)
	{
		printf("Unable to open mouse\n");
	}else
	{
		int flags=fcntl(__mouse_fd,F_GETFL);
		flags |=O_NONBLOCK;
		fcntl(__mouse_fd,F_SETFL,flags);
	}

	return &__mouse[0];
}

static int __dsort (const struct dirent **a,const struct dirent **b) {
    return 1; // dummy sort
}

static int __dfilter(const struct dirent *d) {
    if (d->d_type==DT_DIR) return 0;
    int i=0;
    i=strlen(d->d_name)-1;
    //printf ("%i %c %c %c \n",d->d_type,d->d_name[i-2],d->d_name[i-1],d->d_name[i]);
    // allegedly usb keyboard symlink *always* ends kbd
    if (d->d_name[i-2]=='k'  & d->d_name[i-1]=='b'  & d->d_name[i]=='d'  ) return 1;
    return 0;
}

char*	glpiGetKeysArray()
{
	struct dirent **eps;
	int n;
	int flags;
	char fn[256];

	dflag(1);

	n =scandir("/dev/input/by-path/",&eps,__dfilter,__dsort);

	//dflag(2);
	if(n>0)
	{
		//dflag(n);
		sprintf(fn, "/dev/input/by-path/%s\0",eps[0]->d_name);
		printf("%s\n",fn);
		__key_fd=open(fn,O_RDONLY);

		printf("%i %s\n", __key_fd,fn);
	}

	//dflag(3);

	if(__key_fd==-1)
		__key_fd=0;

	flags = fcntl(__key_fd,F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(__key_fd,F_SETFL,flags);

	if(__key_fd==0)
	{
		struct termios tty_attr;

		if(ioctl(__key_fd,KDGKBMODE,&old_keyboard_mode)<0)
		{
			printf("Could not get keyboard, are you running via ssh?\n");
		}

		tcgetattr(__key_fd,&tty_attr_old);

		tty_attr=tty_attr_old;
		tty_attr.c_lflag &= ~(ICANON | ECHO | ISIG);
		tty_attr.c_iflag &=~(ISTRIP | INLCR | ICRNL | IXON | IXOFF);
		tcsetattr(__key_fd,TCSANOW, &tty_attr);

		ioctl(__key_fd,KDSKBMODE, K_RAW);
	}

	return &__keys[0];
}

void  glpiCloseInput()
{
	if (__mouse_fd!=-1)
 	{
        	close(__mouse_fd);
    	}

	close(__key_fd);
}

int     glpiGetWindowSize(int *x, int *y)
{
    	*x=__display_width;
    	*y=__display_height;

	return 0;
}
