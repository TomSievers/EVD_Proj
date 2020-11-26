#include <include/Boundary/BoundaryEnhancement.hpp>
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
        cv::GaussianBlur(img, img, cv::Size(11,11), 10, 10);
        return nullptr;
    }
} // namespace Detector

