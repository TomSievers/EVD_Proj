#include "include/Change/ChangeSegmentation.hpp"
#include "include/IDetector.hpp"
#include "include/Configure/Configuration.hpp"
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

#ifdef DEBUG
        cv::imshow("Segmented", background);
#endif

        return std::make_shared<cv::Mat>(background);
    }

    cv::Mat ChangeSegmentation::removeBackground(const cv::Mat& image)
    {
        cv::Mat imageBackground;
        cv::inRange(image, Configuration::getInstance().getConfig()->tableColorMin, 
                    Configuration::getInstance().getConfig()->tableColorMax, imageBackground);
        return ~imageBackground;
    }


}