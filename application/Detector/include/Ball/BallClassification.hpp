#ifndef BALL_CLASSIFICATION_HPP
#define BALL_CLASSIFICATION_HPP

#include "include/IImageProcessing.hpp"
#include "include/IDetector.hpp"

namespace Detector
{
    class BallClassification : public IImageProcessing
    {
        public:
            BallClassification();
            virtual ~BallClassification();

            std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
        
        private:
    };
}

#endif