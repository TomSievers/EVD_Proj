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
        std::cout << "FE" << std::endl;
        std::shared_ptr<Config> configPtr = std::static_pointer_cast<Config>(data);
        determinePixelColors(img, *configPtr);
        std::cout << "FE_f" << std::endl;
        return nullptr;
    }

    void TableConfFeatureExtraction::determinePixelColors(cv::Mat& img, Config& config)
    {
        //config.tableColorMin = {179, 255, 255};
        //config.tableColorMax = {0, 0, 0};
        config.tableColorMin = cv::Scalar(179, 255, 255);
        config.tableColorMax = cv::Scalar(0, 0, 0);

        //cv::imshow("img__", img);
        //cv::waitKey(0);

        std::cout << "img rows: " << img.rows << " img cols " << img.cols << std::endl;

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

        std::cout << "config pic: " << (int) config.tableColorMin[0] << " " << (int) config.tableColorMin[1] << " " << (int) config.tableColorMin[2] << std::endl;
        std::cout << "config pic: " << (int) config.tableColorMax[0] << " " << (int) config.tableColorMax[1] << " " << (int) config.tableColorMax[2] << std::endl;
    }
}