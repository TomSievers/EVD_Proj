#ifndef DEBUGDRAWER_HPP
#define DEBUGDRAWER_HPP

#include <include/IImageDrawer.hpp>
#include <opencv2/core.hpp>

namespace ImageDrawer
{
    
    class DebugDrawer : public IImageDrawer
    {
    public:
        DebugDrawer(int width, int height);
        virtual ~DebugDrawer();
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
    private:
        int thickness;
        ColorRGBAInt color;
        ColorRGBInt background;
        cv::Mat img;
        //data
    }; //DebugDrawer
} // namespace ImageDrawer



#endif //DEBUGDRAWER_HPP
