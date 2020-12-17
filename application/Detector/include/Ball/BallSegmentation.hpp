#ifndef BALL_SEGMENTATION_HPP
#define BALL_SEGMENTATION_HPP

#include "include/IImageProcessing.hpp"
#include <opencv2/imgproc.hpp>

namespace Detector
{
    class BallSegmentation : public IImageProcessing
    {
        public:
            BallSegmentation();
            virtual ~BallSegmentation();

            std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);

        private:
            /**
             * @brief function which can be used to find balls in a image.
             * 
             * @param image Binary input image
             * @return std::vector<cv::Vec3f> a vector with all found circles
             */
            std::vector<cv::Vec3f> findCircles(const cv::Mat& image);

            /**
             * @brief function which can be used to remove the pool table background color, to get a image with balls only.
             * 
             * @param image Input image which should be in HSV color format
             * @return cv::Mat A thresholded image where the background color is removed.
             */
            cv::Mat removeBackground(const cv::Mat& image);
    };
}

#endif