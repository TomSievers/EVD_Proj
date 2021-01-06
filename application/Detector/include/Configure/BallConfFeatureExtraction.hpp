#ifndef BALL_CONF_FEATURE_EXTRACTION_HPP
#define BALL_CONF_FEATURE_EXTRACTION_HPP

#include "include/IImageProcessing.hpp"
#include "include/IDetector.hpp"
#include <opencv2/imgproc.hpp>

namespace Detector
{
    class BallConfFeatureExtraction : public IImageProcessing
    {
        public:
            BallConfFeatureExtraction();
            virtual ~BallConfFeatureExtraction();

            std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
        private:
            void determineCueBallPixelValue(cv::Mat& img, Config& config);
    };
}

#endif