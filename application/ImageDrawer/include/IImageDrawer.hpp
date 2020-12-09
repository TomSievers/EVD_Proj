#ifndef IIMAGEDRAWER_HPP
#define IIMAGEDRAWER_HPP

#include <opencv2/core.hpp>

namespace ImageDrawer
{
    struct ColorRGBInt
    {
        int r, b, g;
    };

    struct ColorRGBAInt : public ColorRGBInt
    {
        int a;
    };

    class IImageDrawer
    {
    public:
        virtual void setDrawColor(const ColorRGBInt& color) = 0;
        virtual void setBackground(const ColorRGBInt& color) = 0;
        virtual void drawCircle(const cv::Point& center, double radius) = 0;
        virtual void drawLine(const cv::Point& pointA, const cv::Point& pointB) = 0;
        virtual void setLineWidth(int thickness) = 0;
        virtual void draw() = 0;
        IImageDrawer(){};
        virtual ~IImageDrawer(){};
    private:
        //data
    }; //IImageDrawer

} // namespace ImageDrawer

#endif //IIMAGEDRAWER_HPP
