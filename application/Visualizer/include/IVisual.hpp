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
        CueBall(const cv::Point& center) : center(center), radius(10), thickness(8){};
        CueBall() : center(cv::Point(-1, -1)), radius(10), thickness(8){};
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
            pt_new.x = round((double)(pt.x - inMin.x) * (double)(outMax.x - outMin.x) / (double)(inMax.x - inMin.x) + (double)outMin.x);
            pt_new.y = round((double)(pt.y - inMin.y) * (double)(outMax.y - outMin.y) / (double)(inMax.y - inMin.y) + (double)outMin.y);
            pt_new.x = outMax.x-pt_new.x;
            pt_new.y = outMax.y-pt_new.y;
            return pt_new;
        };

        double mapRad(double rad)
        {
            double factor = ((double)outMax.x)/((double)inMax.x);
            return rad*factor;
        }
        cv::Point inMax;
        cv::Point inMin;
        cv::Point outMax;
        cv::Point outMin;
        std::shared_ptr<ImageDrawer::IImageDrawer> drawer;
    }; //IVisual

} // namespace Visualizer

#endif //IVISUAL_HPP
