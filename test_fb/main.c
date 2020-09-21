#include "functions.h"
#include <assert.h>
#include <signal.h>

struct fb_fix_screeninfo finfo;
struct fb_var_screeninfo vinfo;
size_t size;
uint8_t *fbp;
int fd;
struct termios term;
volatile sig_atomic_t stop;

uint32_t rgbaToInt(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
    return ((r << vinfo.red.offset) | 
        (g << vinfo.green.offset) |
        (b << vinfo.blue.offset) |
        (a << vinfo.transp.offset));
}

void setPixel(uint32_t x, uint32_t y, uint32_t color) {
    uint32_t location = x*vinfo.bits_per_pixel/8 + 
                                    y*finfo.line_length;
    *((uint32_t*) (fbp + location)) = color;
}

void on_sigint(int sig)
{
    set_tty("/dev/tty1", 0);
    set_input(&term, 1);
    close(fd);
    stop = true;
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

    set_input(&term, 0);
    
    fd = open("/dev/fb0", O_RDWR);
    ioctl(fd, FBIOGET_VSCREENINFO, &vinfo);
    ioctl(fd, FBIOGET_FSCREENINFO, &finfo);
    size = vinfo.yres * finfo.line_length;
    fbp = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    uint32_t color = rgbaToInt(0, 0, 0, 0);
    for (uint32_t y = 0; y < vinfo.yres; ++y) 
    {
        for(uint32_t x = 0; x < vinfo.xres; ++x)
        {
            setPixel(x, y, color);
        }
    }

    color = rgbaToInt(0, 0, 0, 0);
    while(!stop)
    {   
        if(is_pressed('c'))
        {
            if(color == rgbaToInt(255, 255, 255, 0))
            {
                color = rgbaToInt(0, 0, 0, 0);
            } else {
                color = rgbaToInt(255, 255, 255, 0);
            }

            for (uint32_t y = vinfo.yres/2-100; y < vinfo.yres/2+100; ++y) 
            {
                for(uint32_t x = vinfo.xres/2-100; x < vinfo.xres/2+100; ++x)
                {
                    setPixel(x, y, color);
                }
            }
        }

        
    }

    return 0;
}
