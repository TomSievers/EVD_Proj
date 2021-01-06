#ifndef TABLE_CONF_SEGMENTATION_HPP
#define TABLE_CONF_SEGMENTATION_HPP

#include "include/IImageProcessing.hpp"
#include "include/IDetector.hpp"
#include <opencv2/imgproc.hpp>

namespace Detector
{
    class TableConfSegmentation : public IImageProcessing
    {
        public:
            TableConfSegmentation();
            virtual ~TableConfSegmentation();

            std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
        private:
            cv::Mat shiftImage(const cv::Mat& img);
            cv::Mat findROI(const cv::Mat& shiftedImage, const cv::Mat& enhancedImage, Config& config);
    };
}

#endif