#ifndef CAIRODRAWER_HPP
#define CAIRODRAWER_HPP

#include <include/IImageDrawer.hpp>
#ifdef __linux__
#include <cairo.h>
#include <linux/fb.h>
#endif

namespace ImageDrawer
{
    enum TTY_MODE
    {
        GRAPHICS,
        TEXT
    };

    class CairoDrawer : public IImageDrawer
    {
    public:
        virtual void setDrawColor(const ColorRGBInt& color);
        virtual void setDrawColor(const ColorRGBAInt& color);
        virtual void setBackground(const ColorRGBInt& color);
        virtual void drawCircle(const cv::Point& center, double radius);
        virtual void drawLine(const cv::Point& pointA, const cv::Point& pointB);
        virtual void setLineWidth(int thickness);
        virtual void draw();
        CairoDrawer(const std::string& framebuffer, const std::string& terminal, cairo_format_t format);
        virtual ~CairoDrawer();
    private:
        void setTty(const std::string& device, TTY_MODE mode);
        unsigned char* fbp;
        int fbfd;
        const std::string terminal;
        unsigned long screensize;
        ColorRGBAInt curColor;
        cv::Point cursorPos;
        uint32_t screenWidth;
        uint32_t screenHeight;
#ifdef __linux__
        fb_var_screeninfo vinfo;
        fb_fix_screeninfo finfo;
        cairo_t* cairoContext;
        cairo_surface_t *cairoSurface;
#endif
        
    }; //CairoDrawer

} // namespace ImageDrawer

#endif //CAIRODRAWER_HPP
