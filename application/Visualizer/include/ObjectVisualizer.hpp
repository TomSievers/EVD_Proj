#ifndef OBJECTVISUALIZER_HPP
#define OBJECTVISUALIZER_HPP
#include <include/IVisual.hpp>
#include <include/CairoDrawer.hpp>

namespace Visualizer
{
    class ObjectVisualizer : public IVisual
    {
    public:
        ObjectVisualizer();
        ObjectVisualizer(const cv::Point& inMin, const cv::Point& inMax);
        ObjectVisualizer(cairo_format_t format, const cv::Point& inMin, const cv::Point& inMax);
        virtual ~ObjectVisualizer();
        virtual void update(const std::vector<cv::Point>& trajectory, const CueBall& ball);
    private:
        std::vector<cv::Point> prevTrajectory;
        CueBall prevBall;
        bool vsync;
    }; //ObjectVisualizer
} // namespace Visualizer

#endif //OBJECTVISUALIZER_HPP
