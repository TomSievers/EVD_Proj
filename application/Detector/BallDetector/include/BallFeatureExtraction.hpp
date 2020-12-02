#ifndef BALL_FEATURE_EXTRACTION_HPP
#define BALL_FEATURE_EXTRACTION_HPP

#include "include/IImageProcessing.hpp"
#include <opencv2/imgproc.hpp>

namespace Detector
{
    struct Contours
    {
        
    };

    class BallFeatureExtraction : public IImageProcessing
    {
        public:
            BallFeatureExtraction();
            virtual ~BallFeatureExtraction();

            cv::Mat getImage(const cv::Mat& image);
            void updateImageData(const cv::Mat& image, std::vector<std::unique_ptr<Object>>& ballObjects);

        private:
            void determineWhitePercentage(const cv::Mat& image, std::vector<std::unique_ptr<Object>>& ballObjects);
            std::vector<Contours> determineContours(const std::vector<std::vector<cv::Point>>& contours, std::vector<std::unique_ptr<Object>>& ballObjects);
            std::vector<cv::Point> findContourCenterPoints(const std::vector<std::vector<cv::Point>>& contours);
    };
}

#endif