#include "include/Configure/BallConfSegmentation.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>

namespace Detector
{
    BallConfSegmentation::BallConfSegmentation()
    {

    }

    BallConfSegmentation::~BallConfSegmentation()
    {

    }

    std::shared_ptr<void> BallConfSegmentation::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        std::shared_ptr<Config> dataPtr = std::static_pointer_cast<Config>(data);
        cv::Mat ROI = img(cv::Rect(dataPtr->roiPositions[0], dataPtr->roiPositions[2], dataPtr->roiPositions[1] - dataPtr->roiPositions[0], dataPtr->roiPositions[3] - dataPtr->roiPositions[2]));
        ROI.copyTo(img);
        return nullptr;
    }

    cv::Mat BallConfSegmentation::removeBackground(const cv::Mat& img)
    {
        cv::Mat hsvImage;
        cv::cvtColor(img, hsvImage, cv::COLOR_BGR2HSV);
        
        cv::Mat channels[3];
        cv::split(hsvImage, channels);

        cv::Mat threshold;
        cv::threshold(channels[2], threshold, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

        cv::Mat hsvWithoutBackground = hsvImage.clone();

        for(int i = 0; i < hsvImage.rows; ++i)
        {
            for(int k = 0; k < hsvImage.cols; ++k)
            {
                if(threshold.at<uchar>(i, k) == 255)
                {
                    hsvWithoutBackground.at<cv::Vec3b>(i, k) = hsvImage.at<cv::Vec3b>(i, k);
                }
                else
                {
                    hsvWithoutBackground.at<cv::Vec3b>(i, k)[0] = 0;
                    hsvWithoutBackground.at<cv::Vec3b>(i, k)[1] = 0;
                    hsvWithoutBackground.at<cv::Vec3b>(i, k)[2] = 0;
                }
                
            }
        }
    }
}