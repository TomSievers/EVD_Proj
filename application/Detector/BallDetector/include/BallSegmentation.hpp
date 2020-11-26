#ifndef BALL_SEGMENTATION_HPP
#define BALL_SEGMENTATION_HPP

#include "include/IImageProcessing.hpp"
#include <opencv2/core/mat.hpp>

namespace Detector
{
    class BallSegmentation : public IImageProcessing
    {
        public:
            BallSegmentation();
            virtual ~BallSegmentation();

            cv::Mat getImage(const cv::Mat& image);
            void updateImageData(const cv::Mat& image, std::vector<std::unique_ptr<Object>>& ballObjects);

        private:
            std::vector<cv::Vec3f> findCircles(const cv::Mat& image);
            cv::Mat removeBackground(const cv::Mat& image);
    };
}

#endif