#ifndef IVISUAL_HPP
#define IVISUAL_HPP
#include <opencv2/core.hpp>
#include <ImageDrawer/include/IImageDrawer.hpp>
#include <memory>

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
        IVisual(std::shared_ptr<ImageDrawer::IImageDrawer> drawer) : drawer(drawer){};
        virtual ~IVisual(){};
        virtual void update(const std::vector<cv::Point>& trajectory, const CueBall& ball) = 0;
    protected:
        std::shared_ptr<ImageDrawer::IImageDrawer> drawer;
    }; //IVisual

} // namespace Visualizer

#endif //IVISUAL_HPP
