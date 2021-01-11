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

            std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
        private:
            cv::Mat blurImage(const cv::Mat& image);
    };
}

#endif