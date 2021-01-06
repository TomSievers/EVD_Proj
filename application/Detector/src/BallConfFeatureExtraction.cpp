#include "include/Configure/BallConfFeatureExtraction.hpp"
#include <opencv2/highgui.hpp>

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
        cv::Mat hsvImage;
        cv::cvtColor(img, hsvImage, cv::COLOR_BGR2HSV);
        
        cv::Mat channels[3];
        cv::split(hsvImage, channels);

        cv::Mat threshold;
        cv::threshold(channels[2], threshold, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

        cv::imshow("threshold", threshold);
        cv::waitKey(0);

        config.cueBallColorMin = cv::Scalar(179, 255, 255);
        config.cueBallColorMax = cv::Scalar(0, 0, 0);

        // check all pixel values
        for(int i = 0; i < hsvImage.rows; ++i)
        {
            for(int k = 0; k < hsvImage.cols; ++k)
            {
                if(threshold.at<uint8_t>(i, k) == 255)
                {
                    for(int x = 0; x < 3; ++x)
                    {
                        if(hsvImage.at<cv::Vec3b>(i, k)[x] < config.cueBallColorMin[x])
                        {
                            config.cueBallColorMin[x] = hsvImage.at<cv::Vec3b>(i, k)[x];
                        }
                        if(hsvImage.at<cv::Vec3b>(i, k)[x] > config.cueBallColorMax[x])
                        {
                            config.cueBallColorMax[x] = hsvImage.at<cv::Vec3b>(i, k)[x];
                        }
                    }
                }
            }
        }
    }
}