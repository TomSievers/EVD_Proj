#ifndef BALL_ENHANCEMENT_HPP
#define BALL_ENHANCEMENT_HPP

#include "include/IImageProcessing.hpp"
#include <opencv2/imgproc.hpp>

namespace Detector
{
    class BallEnhancement : public IImageProcessing
    {
        public:
            BallEnhancement();
            virtual ~BallEnhancement();

            cv::Mat getImage(const cv::Mat& image);
            void updateImageData(const cv::Mat& image, std::vector<std::unique_ptr<Object>>& ballObjects);
        private:
            cv::Mat blurImage(const cv::Mat& image);
    };
}

#endif