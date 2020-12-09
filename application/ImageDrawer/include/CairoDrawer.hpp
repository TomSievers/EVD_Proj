#ifndef CAIRODRAWER_HPP
#define CAIRODRAWER_HPP

#include <include/IImageDrawer.hpp>
#include <cairo.h>

#include <linux/fb.h>

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
        cairo_t* cairoContext;
        cairo_surface_t *cairoSurface;
        const std::string terminal;
        uint32_t screenWidth;
        uint32_t screenHeight;
        fb_var_screeninfo vinfo;
        fb_fix_screeninfo finfo;
        unsigned long screensize;
        ColorRGBAInt curColor;
    }; //CairoDrawer

} // namespace ImageDrawer

#endif //CAIRODRAWER_HPP
