#include "include/Configure/BallConfEnhancement.hpp"

namespace Detector
{
    BallConfEnhancement::BallConfEnhancement()
    {

    }

    BallConfEnhancement::~BallConfEnhancement()
    {

    }

    std::shared_ptr<void> BallConfEnhancement::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        cv::Mat blur = blurImage(img);
        blur.copyTo(img);
        return nullptr;
    }

    cv::Mat BallConfEnhancement::blurImage(const cv::Mat& image)
    {
        cv::Mat blurredImage;
        cv::GaussianBlur(image, blurredImage, cv::Size(3, 3), 0);
        return blurredImage;
    }
}