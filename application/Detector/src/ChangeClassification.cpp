#include "include/Change/ChangeClassification.hpp"
#include <iostream>

#define CHANGE_THRESHOLD 0

namespace Detector
{
    ChangeClassification::ChangeClassification()
    {

    }

    ChangeClassification::~ChangeClassification()
    {

    }

    std::shared_ptr<void> ChangeClassification::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        std::shared_ptr<ChangeObject> objectPtr = std::static_pointer_cast<ChangeObject>(data);
        objectPtr->moving = objectPtr->nonZero > CHANGE_THRESHOLD;
        return nullptr;
    }

   
}