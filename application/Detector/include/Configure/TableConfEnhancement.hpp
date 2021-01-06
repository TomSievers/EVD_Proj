#ifndef TABLE_CONF_ENHANCEMENT_HPP
#define TABLE_CONF_ENHANCEMENT_HPP

#include "include/IImageProcessing.hpp"
#include <opencv2/imgproc.hpp>

namespace Detector
{
    class TableConfEnhancement : public IImageProcessing
    {
        public:
            TableConfEnhancement();
            virtual ~TableConfEnhancement();

            std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
        private:
            cv::Mat blurImage(const cv::Mat& image);
    };
}

#endif