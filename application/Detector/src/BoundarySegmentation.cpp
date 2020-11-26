#include <include/BoundarySegmentation.hpp>
#include <opencv2/imgproc.hpp>

namespace Detector
{
    BoundarySegmentation::BoundarySegmentation()
    {

    }

    BoundarySegmentation::~BoundarySegmentation()
    {
        
    }

    void isolateTableBackground(cv::Mat& img)
    {
        cv::Mat hsv;
        cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);
        cv::inRange(hsv, cv::Scalar(86, 119, 0), cv::Scalar(114, 255, 255), img);
    }

    std::shared_ptr<void> BoundarySegmentation::process(cv::Mat& img)
    {
        isolateTableBackground(img);
        return nullptr;
    }
} // namespace Detector
