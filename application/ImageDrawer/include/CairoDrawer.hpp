#ifndef CAIRODRAWER_HPP
#define CAIRODRAWER_HPP

#include <include/IImageDrawer.hpp>
#if defined(__linux__) && defined(HAVE_CAIRO)
#include <cairo.h>
#include <linux/fb.h>
#else
struct cairo_format_t
{
    int STUB = 0;
};
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
        /**
         * @brief Set the RGB color for every object on the canvas. 
         * 
         * @param color the color RGB of the objects on the canvas
         */
        virtual void setDrawColor(const ColorRGBInt& color);
        /**
         * @brief Set the RGBA color for every object on the canvas. 
         * 
         * @param color the color RGBA of the objects on the canvas
         */
        virtual void setDrawColor(const ColorRGBAInt& color);
        /**
         * @brief Set the Background to a color. Erases all curent objects drawn on screen
         * 
         * @param color 
         */
        virtual void setBackground(const ColorRGBInt& color);
        /**
         * @brief Draw a circle on the screen at the position with a radius
         * 
         * @param center the center of the circle
         * @param radius the radius of the circle
         */
        virtual void drawCircle(const cv::Point& center, double radius);
        /**
         * @brief Draw a line from point a to point b
         * 
         * @param pointA starting point of the line
         * @param pointB end point of the line
         */
        virtual void drawLine(const cv::Point& pointA, const cv::Point& pointB);
        /**
         * @brief Set the Line width of every object on the canvas.
         * 
         * @param thickness the thickness of the lines
         */
        virtual void setLineWidth(int thickness);
        /**
         * @brief Commit drawn shapes to be drawn on the screen
         * 
         */
        virtual void draw();
        /**
         * @brief Construct a new Cairo Drawer object
         * 
         * @param framebuffer path to the file discriptor of the framebuffer
         * @param terminal path to the current terminal connected to the output display
         * @param format color format of the screen
         */
        CairoDrawer(const std::string& framebuffer, const std::string& terminal, cairo_format_t format);
        virtual ~CairoDrawer();
        uint32_t getScreenWidth();
        uint32_t getScreenHeight();
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
#if defined(__linux__) && defined(HAVE_CAIRO)
        fb_var_screeninfo vinfo;
        fb_fix_screeninfo finfo;
        cairo_t* cairoContext;
        cairo_surface_t *cairoSurface;
#endif
        
    }; //CairoDrawer

} // namespace ImageDrawer

#endif //CAIRODRAWER_HPP
