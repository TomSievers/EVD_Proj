#ifndef IVISUAL_HPP
#define IVISUAL_HPP
#include <opencv2/core.hpp>
#include <include/IImageDrawer.hpp>
#include <memory>

namespace Visualizer
{
    
    struct CueBall
    {
        cv::Point center;
        double radius;
        int thickness;
        CueBall(const cv::Point& center) : center(center), radius(10), thickness(4){};
        CueBall() : center(cv::Point(-1, -1)), radius(10), thickness(4){};
        ~CueBall(){};
    };

    class IVisual
    {
    public:
        IVisual(std::shared_ptr<ImageDrawer::IImageDrawer> drawer) : drawer(drawer){};
        virtual ~IVisual(){};
        virtual void update(const std::vector<cv::Point>& trajectory, const CueBall& ball) = 0;
        void setInMaxPoint(const cv::Point& pt){inMax = pt;};
        void setInMinPoint(const cv::Point& pt){inMin = pt;};
        void setOutMaxPoint(const cv::Point& pt){outMax = pt;};
        void setOutMinPoint(const cv::Point& pt){outMin = pt;};
    protected:
        cv::Point map(const cv::Point& pt)
        {
            cv::Point pt_new;
            pt_new.x = (pt.x - inMin.x) * (outMax.x - outMin.x) / (inMax.x - inMin.x) + outMin.x;
            pt_new.y = (pt.y - inMin.y) * (outMax.y - outMin.y) / (inMax.y - inMin.y) + outMin.y;
            return pt_new;
        };
        cv::Point inMax;
        cv::Point inMin;
        cv::Point outMax;
        cv::Point outMin;
        std::shared_ptr<ImageDrawer::IImageDrawer> drawer;
    }; //IVisual

} // namespace Visualizer

#endif //IVISUAL_HPP
