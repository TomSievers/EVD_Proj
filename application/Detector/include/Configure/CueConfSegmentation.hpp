#ifndef CUE_CONF_SEGMENTATION
#define CUE_CONF_SEGMENTATION

#include "include/IImageProcessing.hpp"
#include <opencv2/imgproc.hpp>
#include "include/IDetector.hpp"

namespace Detector
{
    class CueConfSegmentation : public IImageProcessing
    {
        public:
            CueConfSegmentation();
            virtual ~CueConfSegmentation();

            std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
        private:
            cv::Mat removeCueGrip(cv::Mat& img, Config& config);
            void determineLine(const cv::Point& point1, const cv::Point& point2, uint16_t& a, uint16_t& b, uint16_t& c);
            double distancePointAndLine(const cv::Point& point1, const cv::Point& point2, const cv::Point& distancePoint);
    };
}

#endif