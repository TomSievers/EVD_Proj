#include "include/Configure/CueConfFeatureExtraction.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>

namespace Detector
{
    CueConfFeatureExtraction::CueConfFeatureExtraction()
    {

    }

    CueConfFeatureExtraction::~CueConfFeatureExtraction()
    {

    }

    std::shared_ptr<void> CueConfFeatureExtraction::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        std::shared_ptr<Config> dataPtr = std::static_pointer_cast<Config>(data);
        determineCuePixelValue(img, *dataPtr);
        return nullptr;
    }

    void CueConfFeatureExtraction::determineCuePixelValue(cv::Mat& img, Config& config)
    {        
        config.cueColorMin = cv::Scalar(179, 255, 255);
        config.cueColorMax = cv::Scalar(0, 0, 0);

        cv::Mat bgr;
        cv::Mat gray;
        cv::cvtColor(img, bgr, cv::COLOR_HSV2BGR);
        cv::cvtColor(bgr, gray, cv::COLOR_BGR2GRAY);

        cv::Mat threshold;
        cv::threshold(gray, threshold, 0, 255, cv::THRESH_BINARY);

        // check all pixel values
        for(int i = 0; i < img.rows; ++i)
        {
            for(int k = 0; k < img.cols; ++k)
            {
                if(threshold.at<uint8_t>(i, k) == 255)
                {
                    for(int x = 0; x < 3; ++x)
                    {
                        if(img.at<cv::Vec3b>(i, k)[x] < config.cueColorMin[x])
                        {
                            config.cueColorMin[x] = img.at<cv::Vec3b>(i, k)[x];
                        }
                        if(img.at<cv::Vec3b>(i, k)[x] > config.cueColorMax[x])
                        {
                            config.cueColorMax[x] = img.at<cv::Vec3b>(i, k)[x];
                        }
                    }
                }
            }
        }
    }
}