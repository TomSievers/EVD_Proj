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
    CairoDrawer::CairoDrawer(const std::string& framebuffer, const std::string& terminal, cairo_format_t format) : terminal(terminal), screensize(0), curColor(0, 0, 0, 0), cursorPos(0, 0)
    {
#if defined(__linux__) && defined(HAVE_CAIRO)
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
        screenWidth = vinfo.xres;
        screenHeight = vinfo.yres;
#ifdef DEBUG
        std::cout << "Info: Screen format bpp: " << vinfo.bits_per_pixel << std::endl;
        std::cout << "\t red:" << vinfo.red.length << std::endl;
        std::cout << "\t green:" << vinfo.green.length << std::endl;
        std::cout << "\t blue" << vinfo.blue.length << std::endl;
        std::cout << screenWidth << "x" << screenHeight << std::endl;
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
#if defined(__linux__) && defined(HAVE_CAIRO)
        setTty(terminal, TEXT);
        cairo_destroy(cairoContext);
        cairo_surface_finish(cairoSurface);
        cairo_surface_destroy(cairoSurface);
        if(munmap(fbp, screensize) == -1)
        {
            std::cerr << "Failed to unmap framebuffer from memory. errno: " << std::to_string(errno) << std::endl;
        }
        close(fbfd);
#endif
    }

    uint32_t CairoDrawer::getScreenWidth()
    {
        return screenWidth;
    }

    uint32_t CairoDrawer::getScreenHeight()
    {
        return screenHeight;
    }

    void CairoDrawer::setDrawColor(const ColorRGBInt& color)
    {
#if defined(__linux__) && defined(HAVE_CAIRO)
        curColor.r = color.r;
        curColor.b = color.b;
        curColor.g = color.g;
        curColor.a = 255;
        cairo_set_source_rgb(cairoContext, color.r/255.0F, color.b/255.0F, color.g/255.0F);
#endif
    }

    void CairoDrawer::setDrawColor(const ColorRGBAInt& color)
    {
#if defined(__linux__) && defined(HAVE_CAIRO)
        curColor.r = color.r;
        curColor.b = color.b;
        curColor.g = color.g;
        curColor.a = color.a;
        cairo_set_source_rgba(cairoContext, color.r/255.0F, color.b/255.0F, color.g/255.0F, color.a/255.0F);
#endif
    }

    void CairoDrawer::setBackground(const ColorRGBInt& color)
    {
#if defined(__linux__) && defined(HAVE_CAIRO)
        cairo_set_source_rgb(cairoContext, color.r, color.b, color.g);
        cairo_move_to(cairoContext, 0, 0);
        cairo_rectangle(cairoContext, 0, 0, vinfo.xres, vinfo.yres);
        cairo_fill(cairoContext);
        draw();
        cairo_set_source_rgba(cairoContext, curColor.r/255.0F, curColor.b/255.0F, curColor.g/255.0F, curColor.a/255.0F);
        cairo_move_to(cairoContext, cursorPos.x, cursorPos.y);
#endif
    }

    void CairoDrawer::drawCircle(const cv::Point& center, double radius)
    {
#if defined(__linux__) && defined(HAVE_CAIRO)
        cursorPos = center;
        cairo_move_to(cairoContext, center.x+radius, center.y);
        cairo_arc(cairoContext, center.x, center.y, radius, 0.0, 2.0 * M_PI);
        
#endif
    }

    void CairoDrawer::drawLine(const cv::Point& pointA, const cv::Point& pointB)
    {
#if defined(__linux__) && defined(HAVE_CAIRO)
        cursorPos = pointB;
        cairo_move_to(cairoContext, pointA.x, pointA.y);
        cairo_line_to(cairoContext, pointB.x, pointB.y);
#endif
    }

    void CairoDrawer::setLineWidth(int thickness)
    {
#if defined(__linux__) && defined(HAVE_CAIRO)
        cairo_set_line_width(cairoContext, thickness);
#endif
    }

    void CairoDrawer::draw()
    {
#if defined(__linux__) && defined(HAVE_CAIRO)
        cairo_stroke(cairoContext);
#endif
    }
    

    void CairoDrawer::setTty(const std::string& device, TTY_MODE mode)
    {
#if defined(__linux__) && defined(HAVE_CAIRO)
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