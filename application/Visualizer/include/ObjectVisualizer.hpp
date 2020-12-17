#ifndef OBJECTVISUALIZER_HPP
#define OBJECTVISUALIZER_HPP
#include <include/IVisual.hpp>

namespace Visualizer
{
    class ObjectVisualizer : public IVisual
    {
    public:
        ObjectVisualizer();
        virtual ~ObjectVisualizer();
        virtual void update(const std::vector<cv::Point>& trajectory, const CueBall& ball);
    private:
        //data
    }; //ObjectVisualizer
} // namespace Visualizer

#endif //OBJECTVISUALIZER_HPP
