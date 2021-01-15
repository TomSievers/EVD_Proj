#include "include/Configure/TableConfFeatureExtraction.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

namespace Detector
{
    TableConfFeatureExtraction::TableConfFeatureExtraction()
    {

    }

    TableConfFeatureExtraction::~TableConfFeatureExtraction()
    {

    }

    std::shared_ptr<void> TableConfFeatureExtraction::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        std::shared_ptr<Config> configPtr = std::static_pointer_cast<Config>(data);
        determinePixelColors(img, *configPtr);
        return nullptr;
    }

    void TableConfFeatureExtraction::determinePixelColors(cv::Mat& img, Config& config)
    {
        config.tableColorMin = cv::Scalar(179, 255, 255);
        config.tableColorMax = cv::Scalar(0, 0, 0);

        for(int i = 0; i < img.rows; ++i)
        {
            for(int k = 0; k < img.cols; ++k)
            {
                for(int x = 0; x < 3; ++x)
                {
                    if(img.at<cv::Vec3b>(i, k)[x] < config.tableColorMin[x])
                    {
                        config.tableColorMin[x] = img.at<cv::Vec3b>(i, k)[x];
                    }
                    if(img.at<cv::Vec3b>(i, k)[x] > config.tableColorMax[x])
                    {
                        config.tableColorMax[x] = img.at<cv::Vec3b>(i, k)[x];
                    }
                }
            }
        }
    }
}