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
        cv::Mat temp;
        cv::bilateralFilter(img, temp, 7, 90 ,90);
        cv::Mat blur = blurImage(temp);
        blur.copyTo(img);
        return nullptr;
    }

    cv::Mat CueConfEnhancement::blurImage(const cv::Mat& image)
    {
        cv::Mat blurredImage;
        cv::GaussianBlur(image, blurredImage, cv::Size(7, 7), 10, 0);
        return blurredImage;
    }
}