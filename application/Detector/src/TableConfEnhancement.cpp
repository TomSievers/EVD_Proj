#include "include/Configure/TableConfEnhancement.hpp"
#include <opencv2/imgproc.hpp>
#include <iostream>

namespace Detector
{
    TableConfEnhancement::TableConfEnhancement()
    {

    }

    TableConfEnhancement::~TableConfEnhancement()
    {

    }

    std::shared_ptr<void> TableConfEnhancement::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        std::cout << "enhancement" << std::endl;
        cv::Mat blur = blurImage(img);
        blur.copyTo(img);
        std::cout << "enhancement_f" << std::endl;
        return nullptr;
    }

    cv::Mat TableConfEnhancement::blurImage(const cv::Mat& image)
    {
        cv::Mat blurredImage;
        cv::GaussianBlur(image, blurredImage, cv::Size(3, 3), 0);
        return blurredImage;
    }
}