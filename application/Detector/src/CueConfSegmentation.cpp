#include "include/Configure/CueConfSegmentation.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

namespace Detector
{
    CueConfSegmentation::CueConfSegmentation()
    {

    }

    CueConfSegmentation::~CueConfSegmentation()
    {

    }

    std::shared_ptr<void> CueConfSegmentation::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        std::shared_ptr<Config> dataPtr = std::static_pointer_cast<Config>(data);
        cv::Mat ROI = img(cv::Rect(dataPtr->roiPositions[0], dataPtr->roiPositions[2], dataPtr->roiPositions[1] - dataPtr->roiPositions[0], dataPtr->roiPositions[3] - dataPtr->roiPositions[2]));
        cv::Mat imageWithoutGrip = removeCueGrip(ROI, *dataPtr);
        return nullptr;
    }

    cv::Mat CueConfSegmentation::removeCueGrip(cv::Mat& img, Config& config)
    {
        srand(time(0));
        //cv::Mat gray;
        //cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

        cv::Mat hsvImage;
        cv::cvtColor(img, hsvImage, cv::COLOR_BGR2HSV);

        cv::Mat imageRemovedStick;
        cv::inRange(hsvImage, config.tableColorMin, config.tableColorMax, imageRemovedStick);
        
        cv::Mat imageNoBackground;
        cv::bitwise_not(imageRemovedStick, imageNoBackground);

        cv::Mat hsvImageAdjusted = hsvImage.clone();
        for(int i = 0; i < hsvImageAdjusted.rows; ++i)
        {
            for(int k = 0; k < hsvImageAdjusted.cols; ++k)
            {
                if(imageNoBackground.at<uint8_t>(i, k) == 0)
                {
                    hsvImageAdjusted.at<cv::Vec3b>(i, k)[0] = 0;
                    hsvImageAdjusted.at<cv::Vec3b>(i, k)[1] = 0;
                    hsvImageAdjusted.at<cv::Vec3b>(i, k)[2] = 0;
                }
            }
        }

        //cv::Mat channels[3];
        //cv::split(hsvImageAdjusted, channels);

        cv::Mat tooBgr;
        cv::Mat gray;
        cv::cvtColor(hsvImageAdjusted, tooBgr, cv::COLOR_HSV2BGR);
        cv::cvtColor(tooBgr, gray, cv::COLOR_BGR2GRAY);

        cv::Mat cannyImage;
        cv::Canny(gray, cannyImage, 0, 255);


        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(cannyImage, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

        // debug
        cv::Mat contoursDebug;
        cv::cvtColor(gray, contoursDebug, cv::COLOR_GRAY2BGR);

        std::cout << contours.size() << std::endl;

        for(std::size_t i = 0; i < contours.size(); ++i)
        {
            cv::Scalar color( rand()&255, rand()&255, rand()&255 );
            cv::drawContours(contoursDebug, contours, (int)i, color);
            std::cout << cv::contourArea(contours.at(i)) << std::endl;
        }

        /*
        cv::Mat cannyImage;
        cv::Canny(gray, cannyImage, 0, 250);

        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(gray, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

        // debug
        cv::Mat contoursDebug;
        cv::cvtColor(gray, contoursDebug, cv::COLOR_GRAY2BGR);

        std::cout << contours.size() << std::endl;

        for(std::size_t i = 0; i < contours.size(); ++i)
        {
            cv::Scalar color( rand()&255, rand()&255, rand()&255 );
            cv::drawContours(contoursDebug, contours, (int)i, color);
        } */

        cv::Mat resized;
        cv::resize(contoursDebug, resized, cv::Size(), 4, 4);

        cv::imshow( "Contours", resized);
        cv::waitKey(0);
    }
}