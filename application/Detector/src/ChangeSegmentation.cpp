#include "include/Change/ChangeSegmentation.hpp"
#include "include/IDetector.hpp"
#include <cmath>

#include <iostream>
#include <opencv2/highgui.hpp>

namespace Detector
{
    ChangeSegmentation::ChangeSegmentation()
    {

    }

    ChangeSegmentation::~ChangeSegmentation()
    {

    }

    std::shared_ptr<void> ChangeSegmentation::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        cv::Mat imageHSV;
        cv::cvtColor(img, imageHSV, cv::COLOR_BGR2HSV);

        // 1: remove background
        cv::Mat background = removeBackground(imageHSV);
        cv::morphologyEx(background, background, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3)));

        return std::make_shared<cv::Mat>(background);
    }

    cv::Mat ChangeSegmentation::removeBackground(const cv::Mat& image)
    {
        cv::Mat imageBackground;
        cv::inRange(image, cv::Scalar(0, 0, 0), 
                    cv::Scalar(20, 255, 255), imageBackground);
        return imageBackground;
    }


}