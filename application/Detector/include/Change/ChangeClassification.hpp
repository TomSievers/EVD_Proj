#ifndef CHANGE_CLASSIFICATION_HPP
#define CHANGE_CLASSIFICATION_HPP

#include "include/IImageProcessing.hpp"
#include "include/IDetector.hpp"

namespace Detector
{
    class ChangeClassification : public IImageProcessing
    {
        public:
            ChangeClassification();
            virtual ~ChangeClassification();

            std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
    };
}

#endif