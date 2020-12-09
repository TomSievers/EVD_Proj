#ifndef CAIRODRAWER_HPP
#define CAIRODRAWER_HPP

#include <include/IImageDrawer.hpp>
#include <cairo.h>

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
        virtual void setBackground(const ColorRGBInt& color);
        virtual void drawCircle(const cv::Point& center, double radius);
        virtual void drawLine(const cv::Point& pointA, const cv::Point& pointB);
        virtual void setLineWidth(int thickness);
        virtual void draw();
        CairoDrawer(const std::string& framebuffer, const std::string& terminal, cairo_format_t format);
        virtual ~CairoDrawer();
    private:
        bool setTty(const std::string& device, TTY_MODE mode);
        char* fbp;
        cairo_t* cairoContext;
        cairo_surface_t *cairoSurface;
        uint32_t screenWidth;
        uint32_t screenHeight;
    }; //CairoDrawer

} // namespace ImageDrawer

#endif //CAIRODRAWER_HPP
