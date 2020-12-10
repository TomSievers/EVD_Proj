#ifndef IIMAGEDRAWER_HPP
#define IIMAGEDRAWER_HPP

#include <opencv2/core.hpp>

namespace ImageDrawer
{
    struct ColorRGBInt
    {
        uint8_t r, b, g;
        ColorRGBInt(uint8_t r, uint8_t b , uint8_t g) : r(r), b(b), g(g){};
    };

    struct ColorRGBAInt : public ColorRGBInt
    {
        uint8_t a;
        ColorRGBAInt(uint8_t r, uint8_t b , uint8_t g, uint8_t a) : ColorRGBInt(r, b, g), a(a){};
    };

    class IImageDrawer
    {
    public:
        /**
         * @brief Set the RGB color for every object on the canvas. 
         * 
         * @param color the color RGB of the objects on the canvas
         */
        virtual void setDrawColor(const ColorRGBInt& color) = 0;
        /**
         * @brief Set the RGBA color for every object on the canvas. 
         * 
         * @param color the color RGBA of the objects on the canvas
         */
        virtual void setDrawColor(const ColorRGBAInt& color) = 0;
        /**
         * @brief Set the Background to a color. Erases all curent objects drawn on screen
         * 
         * @param color 
         */
        virtual void setBackground(const ColorRGBInt& color) = 0;
        /**
         * @brief Draw a circle on the screen at the position with a radius
         * 
         * @param center the center of the circle
         * @param radius the radius of the circle
         */
        virtual void drawCircle(const cv::Point& center, double radius) = 0;
        /**
         * @brief Draw a line from point a to point b
         * 
         * @param pointA starting point of the line
         * @param pointB end point of the line
         */
        virtual void drawLine(const cv::Point& pointA, const cv::Point& pointB) = 0;
        /**
         * @brief Set the Line width of every object on the canvas.
         * 
         * @param thickness the thickness of the lines
         */
        virtual void setLineWidth(int thickness) = 0;
        /**
         * @brief Commit drawn shapes to be drawn on the screen
         * 
         */
        virtual void draw() = 0;
        IImageDrawer(){};
        virtual ~IImageDrawer(){};
    private:
        //data
    }; //IImageDrawer

} // namespace ImageDrawer

#endif //IIMAGEDRAWER_HPP
