#include "include/Configure/BallConfSegmentation.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>

namespace Detector
{
    BallConfSegmentation::BallConfSegmentation()
    {

    }

    BallConfSegmentation::~BallConfSegmentation()
    {

    }

    std::shared_ptr<void> BallConfSegmentation::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        std::shared_ptr<Config> dataPtr = std::static_pointer_cast<Config>(data);
        cv::Mat ROI = img(cv::Rect(dataPtr->roiPositions[0], dataPtr->roiPositions[2], dataPtr->roiPositions[1] - dataPtr->roiPositions[0], dataPtr->roiPositions[3] - dataPtr->roiPositions[2]));
        ROI.copyTo(img);
        return nullptr;
    }
}