#include "include/Change/ChangeEnhancement.hpp"

#include <iostream>

namespace Detector
{
    ChangeEnhancement::ChangeEnhancement()
    {

    }

    ChangeEnhancement::~ChangeEnhancement()
    {

    }

    std::shared_ptr<void> ChangeEnhancement::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        
        return std::make_shared<cv::Mat>(blurImage(img));
    }

    cv::Mat ChangeEnhancement::blurImage(const cv::Mat& image)
    {
        cv::Mat imgGaussianBlur;
        cv::GaussianBlur(image, imgGaussianBlur, cv::Size(5, 5), 0);
        return imgGaussianBlur;
    }
}