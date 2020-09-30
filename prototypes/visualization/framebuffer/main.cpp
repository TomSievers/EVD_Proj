
extern "C"
{
    #define _XOPEN_SOURCE 700 
    #define _XOPEN_SOURCE_EXTENDED
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
}


struct fb_fix_screeninfo finfo;
struct fb_var_screeninfo vinfo;
size_t size;
uint8_t *fbp;
int fd;

struct point
{
    uint64_t x, y;
};

static bool set_tty(const char* dev, const int mode);

uint32_t rgba_to_int(uint32_t r, uint32_t g, uint32_t b, uint32_t a);

void set_pixel(uint32_t x, uint32_t y, uint32_t color);

void draw_line(point p1, point p2, uint32_t r, uint32_t g, uint32_t b, uint32_t a);

void on_sigint(int sig);

void draw_circle(int xc, int yc, int r);

int main(int argc, char** argv) 
{
    if(signal(SIGINT, on_sigint) == SIG_ERR)
    {
        fprintf(stderr, "unable to assign handler to SIGINT");
        exit(-1);
    }

    if(!set_tty("/dev/tty1", 1))
    {
        exit(-1);
    }
    
    fd = open("/dev/fb0", O_RDWR);
    ioctl(fd, FBIOGET_VSCREENINFO, &vinfo);
    ioctl(fd, FBIOGET_FSCREENINFO, &finfo);
    size = vinfo.yres * finfo.line_length;
    fbp = static_cast<uint8_t*>(mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    
    uint32_t color = rgba_to_int(0, 0, 0, 0);
    for (uint32_t y = 0; y < vinfo.yres; ++y) 
    {
        for(uint32_t x = 0; x < vinfo.xres; ++x)
        {
            set_pixel(x, y, color);
        }
    }

    point a = {.x=0, .y=200};
    point b = {.x=25, .y=300};

    draw_line(a, b, 255, 255, 255, 255);

    draw_circle(500, 500, 100);

    return pause();
}


void draw_circle(int xc, int yc, int r) 
{ 
    uint32_t color = rgba_to_int(255, 0,0,0);
    int x = 0, y = r; 
    int d = 3 - 2 * r; 
    set_pixel(xc+x, yc+y, color); 
    set_pixel(xc-x, yc+y, color); 
    set_pixel(xc+x, yc-y, color); 
    set_pixel(xc-x, yc-y, color); 
    set_pixel(xc+y, yc+x, color); 
    set_pixel(xc-y, yc+x, color); 
    set_pixel(xc+y, yc-x, color); 
    set_pixel(xc-y, yc-x, color); 
    while (y >= x) 
    { 
        // for each pixel we will 
        // draw all eight pixels 
          
        x++; 
  
        // check for decision parameter 
        // and correspondingly  
        // update d, x, y 
        if (d > 0) 
        { 
            y--;  
            d = d + 4 * (x - y) + 10; 
        } 
        else
            d = d + 4 * x + 6; 
        set_pixel(xc+x, yc+y, color); 
        set_pixel(xc-x, yc+y, color); 
        set_pixel(xc+x, yc-y, color); 
        set_pixel(xc-x, yc-y, color); 
        set_pixel(xc+y, yc+x, color); 
        set_pixel(xc-y, yc+x, color); 
        set_pixel(xc+y, yc-x, color); 
        set_pixel(xc-y, yc-x, color); 
    } 
} 

void on_sigint(int sig)
{
    set_tty("/dev/tty1", 0);

    if(munmap(fbp, size) == -1)
    {
        fprintf(stderr, "unable to unmap memory errno: %d\n", errno);
    }

    close(fd);
}

void draw_line(point p1, point p2, uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
    int dx = abs(int(p2.x - p1.x)); 
    int sx = p1.x < p2.x ? 1 : -1;
    int dy = abs(int(p2.y - p1.y)); 
    int sy = p1.y < p2.y ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;

    uint32_t color = rgba_to_int(r, g, b, a); 

    while (set_pixel(p1.x, p1.y, color), p1.x != p2.x || p1.y != p2.y) 
    {
        int e2 = err;
        if (e2 > -dx) { err -= dy; p1.x += sx; }
        if (e2 <  dy) { err += dx; p1.y += sy; }
    }
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

uint32_t rgba_to_int(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
    return ((r << vinfo.red.offset) | 
        (g << vinfo.green.offset) |
        (b << vinfo.blue.offset) |
        (a << vinfo.transp.offset));
}

void set_pixel(uint32_t x, uint32_t y, uint32_t color)
{
    uint32_t location = x*vinfo.bits_per_pixel/8 + 
                                    y*finfo.line_length;
    *((uint32_t*) (fbp + location)) = color;
}
