#include <include/MockAcquisition.hpp>
#include <opencv2/imgcodecs.hpp>

namespace Detector
{
    MockAcquisition::MockAcquisition() : img(cv::imread("../../Photos_pool_table/setup9_3.jpg"))
    {
    }

    MockAcquisition::~MockAcquisition()
    {

    }

    std::shared_ptr<void> MockAcquisition::process(cv::Mat& img)
    {
        img = this->img.clone();
        return nullptr;
    }
    
} // namespace Detector
