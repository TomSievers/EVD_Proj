#ifndef CHANGE_ENHANCEMENT_HPP
#define CHANGE_ENHANCEMENT_HPP

#include "include/IImageProcessing.hpp"
#include <opencv2/imgproc.hpp>

namespace Detector
{
    class ChangeEnhancement : public IImageProcessing
    {
        public:
            ChangeEnhancement();
            virtual ~ChangeEnhancement();

            std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
        private:
            cv::Mat blurImage(const cv::Mat& image);
    };
}

#endif