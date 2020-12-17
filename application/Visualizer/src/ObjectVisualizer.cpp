#include <include/ObjectVisualizer.hpp>
#include <ImageDrawer/include/CairoDrawer.hpp>

namespace Visualizer
{
    
        ObjectVisualizer::ObjectVisualizer() : IVisual(nullptr)
        {
            auto cairo = std::make_shared<ImageDrawer::CairoDrawer>("/dev/fb0", "/dev/tty1", CAIRO_FORMAT_RGB16_565);
            drawer = cairo;
        }

        ObjectVisualizer::~ObjectVisualizer()
        {

        }

        void ObjectVisualizer::update(const std::vector<cv::Point>& trajectory, const CueBall& ball)
        {
            drawer->setLineWidth(1);
            drawer->setDrawColor(ImageDrawer::ColorRGBInt(0, 0, 0));
            for(std::size_t i = 0; i < prevTrajectory.size(); ++i)
            {
                drawer->drawLine(prevTrajectory[i], prevTrajectory[i%prevTrajectory.size()]);
            }
            drawer->draw();
            drawer->setLineWidth(prevBall.thickness);
            drawer->drawCircle(prevBall.center, prevBall.radius);
            drawer->draw();

            drawer->setLineWidth(1);
            drawer->setDrawColor(ImageDrawer::ColorRGBInt(255, 255, 255));
            for(std::size_t i = 0; i < prevTrajectory.size(); ++i)
            {
                drawer->drawLine(prevTrajectory[i], prevTrajectory[i%prevTrajectory.size()]);
            }
            drawer->draw();
            drawer->setLineWidth(prevBall.thickness);
            drawer->drawCircle(prevBall.center, prevBall.radius);
            
        }
    
} // namespace Visualizer