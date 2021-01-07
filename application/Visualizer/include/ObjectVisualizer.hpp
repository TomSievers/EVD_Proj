#ifndef OBJECTVISUALIZER_HPP
#define OBJECTVISUALIZER_HPP
#include <include/IVisual.hpp>
#include <include/CairoDrawer.hpp>

namespace Visualizer
{
    class ObjectVisualizer : public IVisual
    {
    public:
        ObjectVisualizer(cairo_format_t format, const cv::Point& inMin, const cv::Point& inMax);
        virtual ~ObjectVisualizer();
        virtual void update(const std::vector<cv::Point>& trajectory, const CueBall& ball);
    private:
        std::vector<cv::Point> prevTrajectory;
        CueBall prevBall;
    }; //ObjectVisualizer
} // namespace Visualizer

#endif //OBJECTVISUALIZER_HPP
