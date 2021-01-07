#ifndef CHANGE_FEATURE_EXTRACTION_HPP
#define CHANGE_FEATURE_EXTRACTION_HPP

#include "include/IDetector.hpp"
#include "include/IImageProcessing.hpp"
#include <opencv2/imgproc.hpp>

namespace Detector
{
    class ChangeFeatureExtraction : public IImageProcessing
    {
        public:
            ChangeFeatureExtraction();
            virtual ~ChangeFeatureExtraction();

            std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
            void setPreviousFrame(const cv::Mat& img);
        private:
            std::shared_ptr<cv::Mat> previousFrame;
    };
}

#endif