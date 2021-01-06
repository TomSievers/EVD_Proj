#ifndef BALL_CONF_SEGMENTATION_HPP
#define BALL_CONF_SEGMENTATION_HPP

#include "include/IImageProcessing.hpp"
#include "include/IDetector.hpp"
#include <opencv2/imgproc.hpp>

namespace Detector
{
    class BallConfSegmentation : public IImageProcessing
    {
        public:
            BallConfSegmentation();
            virtual ~BallConfSegmentation();

            std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
    };
}

#endif