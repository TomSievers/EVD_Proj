#include <include/CairoDrawer.hpp>
#include <iostream>
#ifdef __linux__
#include <fcntl.h> 
#include <sys/ioctl.h>
#include <sys/kd.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <linux/mman.h>
#include <math.h>
#endif

namespace ImageDrawer
{
    CairoDrawer::CairoDrawer(const std::string& framebuffer, const std::string& terminal, cairo_format_t format) : terminal(terminal), screensize(0), curColor(0, 0, 0, 0)
    {
#ifdef __linux__
        setTty(terminal, GRAPHICS);
        fbfd = open (framebuffer.c_str(), O_RDWR );
        if (fbfd == -1) {
            throw(std::runtime_error("Can't open framebuffer descriptor device: " + framebuffer));
        }

        if (ioctl (fbfd, FBIOGET_FSCREENINFO, &finfo)) {
            close (fbfd);
            throw(std::runtime_error("Error reading fixed framebuffer information"));
        }

        if (ioctl (fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
            close (fbfd);
            throw(std::runtime_error("Error reading variable framebuffer information"));
        }

        screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
#ifdef DEBUG
        std::cout << "Info: Screen format bpp: " << vinfo.bits_per_pixel << std::endl;
        std::cout << "\t red:" << vinfo.red.length << std::endl;
        std::cout << "\t green:" << vinfo.green.length << std::endl;
        std::cout << "\t blue" << vinfo.blue.length << std::endl;
#endif

        fbp = (unsigned char*) mmap (0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

        if (fbp == NULL) {
            close (fbfd);
            throw(std::runtime_error("Failed to map framebuffer into memory"));
        }

        cairoSurface = cairo_image_surface_create_for_data(fbp, format, vinfo.xres, vinfo.yres, finfo.line_length); 
        cairoContext = cairo_create(cairoSurface);
        cairo_move_to(cairoContext, 0, 0);

#endif
    }

    CairoDrawer::~CairoDrawer()
    {
#ifdef __linux__
        setTty(terminal, TEXT);
        cairo_destroy(cairoContext);
        cairo_surface_finish(cairoSurface);
        cairo_surface_destroy(cairoSurface);
        if(munmap(fbp, screensize) == -1)
        {
            throw(std::runtime_error("Failed to unmap framebuffer from memory. errno: " + std::to_string(errno)));
        }
        close(fbfd);
#endif
    }

    void CairoDrawer::setDrawColor(const ColorRGBInt& color)
    {
#ifdef __linux__
        curColor.r = color.r;
        curColor.b = color.b;
        curColor.g = color.g;
        curColor.a = 255;
        cairo_set_source_rgb(cairoContext, color.r/255.0F, color.b/255.0F, color.g/255.0F);
#endif
    }

    void CairoDrawer::setDrawColor(const ColorRGBAInt& color)
    {
#ifdef __linux__
        curColor.r = color.r;
        curColor.b = color.b;
        curColor.g = color.g;
        curColor.a = color.a;
        cairo_set_source_rgba(cairoContext, color.r/255.0F, color.b/255.0F, color.g/255.0F, color.a/255.0F);
#endif
    }

    void CairoDrawer::setBackground(const ColorRGBInt& color)
    {
#ifdef __linux__
        cairo_set_source_rgb(cairoContext, color.r, color.b, color.g);
        cairo_move_to(cairoContext, 0, 0);
        cairo_rectangle(cairoContext, 0, 0, vinfo.xres, vinfo.yres);
        cairo_fill(cairoContext);
        cairo_set_source_rgba(cairoContext, curColor.r/255.0F, curColor.b/255.0F, curColor.g/255.0F, curColor.a/255.0F);
#endif
    }

    void CairoDrawer::drawCircle(const cv::Point& center, double radius)
    {
#ifdef __linux__
        cairo_move_to(cairoContext, center.x+radius, center.y);
        cairo_arc(cairoContext, center.x, center.y, radius, 0.0, 2.0 * M_PI);
#endif
    }

    void CairoDrawer::drawLine(const cv::Point& pointA, const cv::Point& pointB)
    {
#ifdef __linux__
        cairo_move_to(cairoContext, pointA.x, pointA.y);
        cairo_line_to(cairoContext, pointB.x, pointB.y);
#endif
    }

    void CairoDrawer::setLineWidth(int thickness)
    {
#ifdef __linux__
        cairo_set_line_width(cairoContext, thickness);
#endif
    }

    void CairoDrawer::draw()
    {
#ifdef __linux__
        cairo_stroke(cairoContext);
#endif
    }
    

    void CairoDrawer::setTty(const std::string& device, TTY_MODE mode)
    {
#ifdef __linux__
        int fd = open(device.c_str(), O_RDWR);

        if (fd == -1) {
            throw(std::runtime_error("Could not open terminal: " + device + " errno: " + std::to_string(errno)));
        }

        switch (mode)
        {
        case GRAPHICS:
            {
                if (ioctl( fd, KDSETMODE, KD_GRAPHICS))
                {
                    close(fd);
                    throw(std::runtime_error("Could not set terminal to KD_GRAPHICS mode: " + device + " errno: " + std::to_string(errno)));
                }
            }
            break;
        case TEXT:
            {
                if (ioctl( fd, KDSETMODE, KD_TEXT))
                {
                    close(fd);
                    throw(std::runtime_error("Could not set terminal to KD_GRAPHICS mode: " + device + " errno: " + std::to_string(errno)));
                }
            }
            break;
        default:
            break;
        }
        close(fd);
#endif
    }

} // namespace ImageDrawer