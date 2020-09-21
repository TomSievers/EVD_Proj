#include "functions.h"
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
#include <signal.h>

struct fb_fix_screeninfo finfo;
struct fb_var_screeninfo vinfo;
size_t size;
uint8_t *fbp;
int fd;


void setPixel(uint32_t x, uint32_t y, uint32_t r,
                uint32_t g, uint32_t b, uint32_t a) {
    uint32_t pixel = (r << vinfo.red.offset)|
                    (g << vinfo.green.offset)|
                    (b << vinfo.blue.offset)|
                    (a << vinfo.transp.offset);
    uint32_t location = x*vinfo.bits_per_pixel/8 + 
                                    y*finfo.line_length;
    *((uint32_t*) (fbp + location)) = pixel;
}

void on_sigint(int sig)
{
    set_tty("/dev/tty1", 0);
    close(fd);
    exit(-1);
}

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
    fbp = mmap(0, size, PROT_READ |
                PROT_WRITE, MAP_SHARED, fd, 0);

    uint32_t x = 0;
    uint32_t y = 0;
    for (y = 0; y < vinfo.yres; ++y) 
    {
        for(x = 0; x < vinfo.xres; ++x)
        {
            setPixel(x, y, 0, 0, 0, 0);
        }
    }

    return pause();
}
