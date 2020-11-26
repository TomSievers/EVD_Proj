#ifndef IIMAGE_PROCESSING_HPP
#define IIMAGE_PROCESSING_HPP

#include "include/Object.hpp"
#include <opencv2/core/mat.hpp>
#include <vector>
#include <memory>

namespace Detector
{
    class IImageProcessing
    {
        public:
            IImageProcessing() {};
            virtual ~IImageProcessing() {};

            virtual cv::Mat getImage(const cv::Mat& image) = 0;
            virtual void updateImageData(const cv::Mat& image, std::vector<std::unique_ptr<Object>>& ballObjects) = 0;
    };

}
#endif