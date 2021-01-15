#include "include/Configure/BallConfFeatureExtraction.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>

namespace Detector
{
    BallConfFeatureExtraction::BallConfFeatureExtraction()
    {

    }

    BallConfFeatureExtraction::~BallConfFeatureExtraction()
    {

    }

    std::shared_ptr<void> BallConfFeatureExtraction::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        std::shared_ptr<Config> dataPtr = std::static_pointer_cast<Config>(data);
        determineCueBallPixelValue(img, *dataPtr);
        return nullptr;
    }

    void BallConfFeatureExtraction::determineCueBallPixelValue(cv::Mat& img, Config& config)
    {        
        config.cueBallColorMin = cv::Scalar(179, 255, 255);
        config.cueBallColorMax = cv::Scalar(0, 0, 0);

        // check all pixel values
        for(int i = 0; i < img.rows; ++i)
        {
            for(int k = 0; k < img.cols; ++k)
            {
                if(img.at<cv::Vec3b>(i, k)[0] != 0 && img.at<cv::Vec3b>(i, k)[1] != 0 && img.at<cv::Vec3b>(i, k)[1] != 0)
                {
                    for(int x = 0; x < 3; ++x)
                    {
                        if(img.at<cv::Vec3b>(i, k)[x] < config.cueBallColorMin[x])
                        {
                            config.cueBallColorMin[x] = img.at<cv::Vec3b>(i, k)[x];
                        }
                        if(img.at<cv::Vec3b>(i, k)[x] > config.cueBallColorMax[x])
                        {
                            config.cueBallColorMax[x] = img.at<cv::Vec3b>(i, k)[x];
                        }
                    }
                }
            }
        }
    }
}