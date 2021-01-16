#include <include/ObjectVisualizer.hpp>
#include <include/DebugDrawer.hpp>
#include <include/VsyncCairoDrawer.hpp>
#include <iostream>

namespace Visualizer
{
    ObjectVisualizer::ObjectVisualizer() : IVisual(nullptr), vsync(true)
    {
        auto vsync = std::make_shared<ImageDrawer::VsyncCairoDrawer>("/dev/dri/card1", "/dev/tty1");
        drawer = vsync;
        outMin = cv::Point(0, 0);
        outMin = cv::Point(0, 0);
        this->inMin = inMin;


        outMax = cv::Point(vsync->getScreenWidth(), vsync->getScreenHeight());
        this->inMax = inMax;
        drawer->setBackground(ImageDrawer::ColorRGBInt(0, 0, 0));
    }

    ObjectVisualizer::ObjectVisualizer(const cv::Point& inMin, const cv::Point& inMax) : IVisual(nullptr), vsync(false)
    {
        auto debug = std::make_shared<ImageDrawer::DebugDrawer>(inMax.x, inMax.y);
        drawer = debug;
        outMin = cv::Point(0, 0);
        this->inMin = inMin;


        outMax = inMax;
        this->inMax = inMax;
        drawer->setBackground(ImageDrawer::ColorRGBInt(0, 0, 0));
    }
    
    ObjectVisualizer::ObjectVisualizer(cairo_format_t format, const cv::Point& inMin, const cv::Point& inMax) : IVisual(nullptr), vsync(false)
    {
        auto cairo = std::make_shared<ImageDrawer::CairoDrawer>("/dev/fb0", "/dev/tty1", format);
        drawer = cairo;
        outMin = cv::Point(0, 0);
        this->inMin = inMin;


        outMax = cv::Point(cairo->getScreenWidth(), cairo->getScreenHeight());
        this->inMax = inMax;
        drawer->setBackground(ImageDrawer::ColorRGBInt(0, 0, 0));
    }

    ObjectVisualizer::~ObjectVisualizer()
    {

    }

    void ObjectVisualizer::update(const std::vector<cv::Point>& trajectory, const CueBall& ball)
    {
        drawer->setLineWidth(3);
        drawer->setDrawColor(ImageDrawer::ColorRGBInt(0, 0, 0));
        if(!prevTrajectory.empty())
        {
            for(std::size_t i = 0; i < prevTrajectory.size()-1; ++i)
            {
                drawer->drawLine(map(prevTrajectory.at(i)), map(prevTrajectory.at(i+1)));
            }
            drawer->draw();
        }
        if(prevBall.center != cv::Point(-1, -1))
        {
            drawer->setLineWidth(prevBall.thickness*2);
            drawer->drawCircle(map(prevBall.center), mapRad(prevBall.radius));
            drawer->draw();
        }
        

        drawer->setLineWidth(2);
        drawer->setDrawColor(ImageDrawer::ColorRGBInt(255, 255, 255));
        if(!trajectory.empty())
        {
            for(std::size_t i = 0; i < trajectory.size()-1; ++i)
            {
                drawer->drawLine(map(trajectory.at(i)), map(trajectory.at(i+1)));
            }
            drawer->draw();
        }
        if(ball.center != cv::Point(-1, -1))
        {
            drawer->setLineWidth(ball.thickness);
            drawer->drawCircle(map(ball.center), mapRad(ball.radius));
            drawer->draw();
        }

        prevTrajectory = trajectory;
        prevBall = ball;
        
    }
    
} // namespace Visualizer