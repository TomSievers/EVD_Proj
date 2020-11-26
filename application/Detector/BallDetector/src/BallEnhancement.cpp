#include "include/BallEnhancement.hpp"

namespace Detector
{
    BallEnhancement::BallEnhancement()
    {

    }

    BallEnhancement::~BallEnhancement()
    {

    }

    cv::Mat BallEnhancement::getImage(const cv::Mat& image)
    {
        return blurImage(image);
    }

    void BallEnhancement::updateImageData(const cv::Mat& image, std::vector<std::unique_ptr<Object>>& ballObjects)
    {
        // not implemented
    }

    cv::Mat BallEnhancement::blurImage(const cv::Mat& image)
    {
        cv::Mat imgGaussianBlur;
        cv::GaussianBlur(image, imgGaussianBlur, cv::Size(5, 5), 0);
        return imgGaussianBlur;
    }
}