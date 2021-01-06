#ifndef CUE_CONF_ENHANCEMENT
#define CUE_CONF_ENHANCEMENT

#include "include/IImageProcessing.hpp"
#include <opencv2/imgproc.hpp>

namespace Detector
{
    class CueConfEnhancement : public IImageProcessing
    {
        public:
            CueConfEnhancement();
            virtual ~CueConfEnhancement();

            std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
        private:
            cv::Mat blurImage(const cv::Mat& image);
    };
}

#endif