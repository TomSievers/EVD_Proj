#ifndef CUE_CONF_FEATURE_EXTRACTION
#define CUE_CONF_FEATURE_EXTRACTION

#include "include/IImageProcessing.hpp"
#include <opencv2/imgproc.hpp>

namespace Detector
{
    class CueConfFeatureExtraction : public IImageProcessing
    {
        public:
            CueConfFeatureExtraction();
            virtual ~CueConfFeatureExtraction();

            std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
    };
}

#endif