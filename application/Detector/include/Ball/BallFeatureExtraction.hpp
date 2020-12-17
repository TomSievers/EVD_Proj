#ifndef BALL_FEATURE_EXTRACTION_HPP
#define BALL_FEATURE_EXTRACTION_HPP

#include "include/IDetector.hpp"
#include "include/IImageProcessing.hpp"
#include <opencv2/imgproc.hpp>

namespace Detector
{
    class BallFeatureExtraction : public IImageProcessing
    {
        public:
            BallFeatureExtraction();
            virtual ~BallFeatureExtraction();

            std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
            void setEnhancementImage(const cv::Mat& img);
        private:
            void determineWhitePercentage(const cv::Mat& image, std::vector<std::shared_ptr<BallObject>>& ballObjects);
            std::shared_ptr<cv::Mat> enhancementImg;
    };
}

#endif