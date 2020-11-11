#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <fcntl.h> 
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/kd.h>
#include <errno.h>
#include <assert.h>
#include <stdbool.h>
#include <zlib.h>
#include <stdint.h>
#include <termios.h>
#include <signal.h>
#include <assert.h>
#include <signal.h>

static EGLint const attribute_list[] = {
        EGL_RED_SIZE, 1,
        EGL_GREEN_SIZE, 1,
        EGL_BLUE_SIZE, 1,
        EGL_NONE
};

struct fb_window {
  int width;
  int height;
  int posx;
  int posy;
};

int main(int argc, char const *argv[])
{
    
    EGLDisplay display;
    EGLConfig config;
    EGLContext context;
    EGLSurface surface;
    NativeWindowType native_window;
    EGLint num_config;

    int fb_dpy = open("/dev/fb0", O_RDWR);

    struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;

    if (ioctl (fb_dpy, FBIOGET_FSCREENINFO, &finfo)) {
		printf ("Error reading fixed information\n");
		close (fb_dpy);
		exit (2);
	}

	if (ioctl (fb_dpy, FBIOGET_VSCREENINFO, &vinfo)) {
		printf ("Error reading variable information\n");
		close (fb_dpy);
		exit (3);
	}

    setenv("EGL_PLATFORM", "fbdev", 1);
    display = eglGetDisplay((EGLNativeDisplayType)(long)fb_dpy);

    eglInitialize(display, NULL, NULL);

    eglBindAPI(EGL_OPENGL_ES_API);

    /* get an appropriate EGL frame buffer configuration */
    eglChooseConfig(display, attribute_list, &config, 1, &num_config);

    /* create an EGL rendering context */
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, NULL);

    struct fb_window* fb_win = calloc(1, sizeof(struct fb_window));
    if (!fb_win) {
      printf("fb_window calloc failed: %d\n", errno);
      exit(0);
    }

    fb_win->width = vinfo.xres;
    fb_win->height = vinfo.yres;
    fb_win->posx = 0;
    fb_win->posy = 0;

    surface = eglCreateWindowSurface(display, config, native_window, NULL);

    EGLContext egl_ctx = eglCreateContext(display, config, EGL_NO_CONTEXT, NULL);

    EGLBoolean err = eglMakeCurrent(display, surface, surface, egl_ctx);

        /* connect the context to the surface */
    eglMakeCurrent(display, surface, surface, context);

        /* clear the color buffer */

    eglSwapBuffers(display, surface);

    sleep(10);
    return EXIT_SUCCESS;
}
