#include "include/BallAcquisition.hpp"

namespace Detector
{
    ImageCapture::Capture BallAcquisition::capture(0);

    BallAcquisition::BallAcquisition()
    {

    }

    BallAcquisition::~BallAcquisition()
    {

    }

    cv::Mat BallAcquisition::getImage(const cv::Mat& image)
    {
        cv::Mat acquiredImg;
        while(acquiredImg.empty())
        {
            acquiredImg = capture.getFrame();
        }
        return acquiredImg;
    }

    void BallAcquisition::updateImageData(const cv::Mat& image, std::vector<std::unique_ptr<Object>>& ballObjects)
    {
        // not implemented
    }
}