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
        
        cv::Mat kernel = (cv::Mat_<float>(3,3) <<
                1,  1, 1,
                1, -8, 1,
                1,  1, 1); 
        cv::Mat imgLaplacian;
        cv::filter2D(img, imgLaplacian, CV_32F, kernel);
        cv::Mat sharp;
        img.convertTo(sharp, CV_32F);
        cv::Mat imgResult = sharp - imgLaplacian;

        imgResult.convertTo(imgResult, CV_8UC3);

        cv::Mat temp;
        cv::bilateralFilter(imgResult, temp, 7, 90 ,90);
        return std::make_shared<cv::Mat>(blurImage(temp));
    }

    cv::Mat BallEnhancement::blurImage(const cv::Mat& image)
    {
        
        cv::Mat imgGaussianBlur;
        cv::GaussianBlur(image, imgGaussianBlur, cv::Size(7, 7), 0);
        return imgGaussianBlur;
    }
}