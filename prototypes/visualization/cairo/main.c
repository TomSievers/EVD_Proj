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
#include <cairo.h>
#include <math.h>

int fbfd = 0;
char* fbp = 0;
long int screensize = 0;

void on_sigint(int sig);
static bool set_tty(const char* dev, const int mode);

int main()
{
	
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	cairo_t* cr;
	cairo_surface_t *surface;

    if(signal(SIGINT, on_sigint) == SIG_ERR)
    {
        fprintf(stderr, "unable to assign handler to SIGINT");
        exit(-1);
    }

    if(!set_tty("/dev/tty1", 1))
    {
        exit(-1);
    }

	// open the frame buffer file for reading & writing
	fbfd = open ( "/dev/fb0", O_RDWR );
	if (!fbfd) {
		printf ("Error: can't open framebuffer device.\n");
		exit (1);
	}
	printf ("The framebuffer device was opened successfully\n");

	if (ioctl (fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		printf ("Error reading fixed information\n");
		close (fbfd);
		exit (2);
	}

	if (ioctl (fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		printf ("Error reading variable information\n");
		close (fbfd);
		exit (3);
	}

	// print info about the buffer
	printf ("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

	// calculates size
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	// map the device to memory 
	fbp = (char*) mmap (0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

	if ((int)fbp == -1) {
		printf ("Error: failed to map framebuffer device to memory\n");
		close (fbfd);
		exit (4);
	}

	printf ("The framebuffer device was successfully mapped to memory\n");

	surface = cairo_image_surface_create_for_data (fbp, CAIRO_FORMAT_RGB16_565, vinfo.xres, vinfo.yres, finfo.line_length); 
	cr = cairo_create (surface);
    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_move_to(cr, 0, 0);
    cairo_line_to (cr, vinfo.xres-1, 0);
    cairo_line_to (cr, vinfo.xres-1, vinfo.yres-1);
    cairo_line_to (cr, 0, vinfo.yres-1);
    cairo_line_to (cr, 0, 0);

    cairo_fill(cr);

    cairo_set_source_rgb (cr, 255, 255, 255);
	cairo_move_to (cr, 100, 100);
	cairo_line_to (cr, 300, 300);

    cairo_move_to(cr, 600, 500);
    cairo_arc(cr, 500, 500, 100, 0.0, 2.0 * M_PI);
    cairo_stroke(cr);
	
	return pause();
}

void on_sigint(int sig)
{
    set_tty("/dev/tty1", 0);

    if(munmap(fbp, screensize) == -1)
    {
        fprintf(stderr, "unable to unmap memory errno: %d\n", errno);
    }

    close(fbfd);
}

static bool set_tty(const char* dev, const int mode)
{
    int con_fd = open(dev, O_RDWR);
	bool suc = true;

    if (!con_fd) {
        fprintf(stderr,"Could not open console.\n");
		suc = false;
    }

    if (mode)
	{
		if (ioctl( con_fd, KDSETMODE, KD_GRAPHICS))
		{
			fprintf(stderr,"Could not set console to KD_GRAPHICS mode, err: %d\n", errno);
			suc = false;
		}
	} else {
		if (ioctl( con_fd, KDSETMODE, KD_TEXT))
		{
			fprintf(stderr,"Could not set console to KD_TEXT mode, err: %d\n", errno);
			suc = false;
		}
	}

    close(con_fd);
	return suc;
}

