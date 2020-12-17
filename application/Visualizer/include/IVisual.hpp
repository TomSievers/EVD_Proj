#ifndef IVISUAL_HPP
#define IVISUAL_HPP
#include <opencv2/core.hpp>

namespace Visualizer
{
    
    struct CueBall
    {
        cv::Point center;
        double radius = 10;
        int thickness = 4;
    };

    class IVisual
    {
    public:
        IVisual();
        virtual ~IVisual();
        virtual void update(const std::vector<cv::Point>& trajectory, const CueBall& ball) = 0;
    private:
    }; //IVisual

} // namespace Visualizer

#endif //IVISUAL_HPP
