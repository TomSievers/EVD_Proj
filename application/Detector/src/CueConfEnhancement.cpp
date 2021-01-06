#include "include/Configure/CueConfEnhancement.hpp"

namespace Detector
{
    CueConfEnhancement::CueConfEnhancement()
    {

    }

    CueConfEnhancement::~CueConfEnhancement()
    {

    }

    std::shared_ptr<void> CueConfEnhancement::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        cv::Mat blur = blurImage(img);
        blur.copyTo(img);
        return nullptr;
    }

    cv::Mat CueConfEnhancement::blurImage(const cv::Mat& image)
    {
        cv::Mat blurredImage;
        cv::GaussianBlur(image, blurredImage, cv::Size(3, 3), 0);
        return blurredImage;
    }
}