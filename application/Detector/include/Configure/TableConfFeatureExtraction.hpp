#ifndef TABLE_CONF_FEATURE_EXTRACTION_HPP
#define TABLE_CONF_FEATURE_EXTRACTION_HPP

#include "include/IImageProcessing.hpp"
#include "include/IDetector.hpp"
#include <opencv2/imgproc.hpp>

namespace Detector
{
    class TableConfFeatureExtraction : public IImageProcessing
    {
        public:
            TableConfFeatureExtraction();
            virtual ~TableConfFeatureExtraction();

            std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
        private:
            void determinePixelColors(cv::Mat& img, Config& config);
    };
}

#endif