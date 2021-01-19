#include <include/Boundary/BoundarySegmentation.hpp>
#include <opencv2/imgproc.hpp>


#define BACKGROUND_MIN_H 80
#define BACKGROUND_MIN_S 240
#define BACKGROUND_MIN_V 85
#define BACKGROUND_MAX_H 120
#define BACKGROUND_MAX_S 255
#define BACKGROUND_MAX_V 195


namespace Detector
{
    BoundarySegmentation::BoundarySegmentation()
    {

    }

    BoundarySegmentation::~BoundarySegmentation()
    {
        
    }

    void BoundarySegmentation::isolateTableBackground(cv::Mat& img)
    {
        cv::Mat hsv;
        cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);
        cv::inRange(hsv, cv::Scalar(BACKGROUND_MIN_H, BACKGROUND_MIN_S, BACKGROUND_MIN_V), cv::Scalar(BACKGROUND_MAX_H, BACKGROUND_MAX_S, BACKGROUND_MAX_V), img);
    }

    std::shared_ptr<void> BoundarySegmentation::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        isolateTableBackground(img);
        auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(150, 150));
        cv::morphologyEx(img, img, cv::MORPH_CLOSE, kernel);
        return nullptr;
    }
} // namespace Detector
