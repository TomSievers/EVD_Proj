#include <include/BoundaryEnhancement.hpp>
#include <opencv2/imgproc.hpp>

namespace Detector
{
    BoundaryEnhancement::BoundaryEnhancement()
    {

    }

    BoundaryEnhancement::~BoundaryEnhancement()
    {
        
    }

    std::shared_ptr<void> BoundaryEnhancement::process(cv::Mat& img)
    {
        cv::GaussianBlur(img, img, cv::Size(3,3), 10, 0);
        return nullptr;
    }
} // namespace Detector

