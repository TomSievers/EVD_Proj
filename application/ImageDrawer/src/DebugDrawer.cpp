#include <include/DebugDrawer.hpp>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

namespace ImageDrawer
{
    DebugDrawer::DebugDrawer(int width, int height) : thickness(1), color(0, 0, 0, 0), background(0, 0, 0)
    {
        img = cv::Mat::zeros(cv::Size(width, height), CV_8UC4);
    }

    DebugDrawer::~DebugDrawer()
    {

    }

    void DebugDrawer::setDrawColor(const ColorRGBInt& color)
    {
        this->color.r = color.r;
        this->color.g = color.g;
        this->color.b = color.b;
        this->color.a = 255;
    }

    void DebugDrawer::setDrawColor(const ColorRGBAInt& color)
    {
        this->color.r = color.r;
        this->color.g = color.g;
        this->color.b = color.b;
        this->color.a = color.a;
    }

    void DebugDrawer::setBackground(const ColorRGBInt& color)
    {
        background.r = color.r;
        background.g = color.g;
        background.b = color.b;
        cv::floodFill(img, cv::Point(0, 0), cv::Scalar(background.b, background.g, background.r));
    }

    void DebugDrawer::drawCircle(const cv::Point& center, double radius)
    {
        cv::circle(img, center, radius, cv::Scalar(color.b, color.g, color.r, color.a), thickness);
    }

    void DebugDrawer::drawLine(const cv::Point& pointA, const cv::Point& pointB)
    {
        cv::line(img, pointA, pointB, cv::Scalar(color.b, color.g, color.r, color.a), thickness);
    }

    void DebugDrawer::setLineWidth(int thickness)
    {
        this->thickness = thickness;
    }

    void DebugDrawer::draw()
    {
        cv::imshow("output", img);
    }

} // namespace ImageDrawer