#include <include/DebugDrawer.hpp>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

namespace ImageDrawer
{
    DebugDrawer::DebugDrawer(int width, int height) : thickness(1), color(0, 0, 0, 0), background(0, 0, 0), width(width), height(height)
    {
        img = cv::Mat::zeros(cv::Size(width, height), CV_8UC3);
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
        img = cv::Mat::zeros(cv::Size(width, height), CV_8UC3);
        background.r = color.r;
        background.g = color.g;
        background.b = color.b;
        cv::floodFill(img, cv::Point(0, 0), cv::Scalar(background.b, background.g, background.r));
    }

    void DebugDrawer::drawCircle(const cv::Point& center, double radius)
    {
        cv::circle(img, center, (int)round(radius), cv::Scalar(color.b, color.g, color.r), thickness);
    }

    void DebugDrawer::drawLine(const cv::Point& pointA, const cv::Point& pointB)
    {
        cv::line(img, pointA, pointB, cv::Scalar(color.b, color.g, color.r), thickness);
    }

    void DebugDrawer::setLineWidth(int thickness)
    {
        this->thickness = thickness;
    }

    void DebugDrawer::draw()
    {
        cv::imshow("output", img);
        cv::waitKey(1);
    }

} // namespace ImageDrawer