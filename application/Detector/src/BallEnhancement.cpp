#include "include/Ball/BallEnhancement.hpp"

#include <iostream>

namespace Detector
{
    BallEnhancement::BallEnhancement()
    {

    }

    BallEnhancement::~BallEnhancement()
    {

    }

    std::shared_ptr<void> BallEnhancement::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        return std::make_shared<cv::Mat>((img));
    }

    cv::Mat BallEnhancement::blurImage(const cv::Mat& image)
    {
        cv::Mat imgGaussianBlur;
        cv::GaussianBlur(image, imgGaussianBlur, cv::Size(5, 5), 0);
        return imgGaussianBlur;
    }
}