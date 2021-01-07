#include "include/Change/ChangeFeatureExtraction.hpp"

#include <opencv2/highgui.hpp>
#include <iostream>

namespace Detector
{
    ChangeFeatureExtraction::ChangeFeatureExtraction()
    {

    }

    ChangeFeatureExtraction::~ChangeFeatureExtraction()
    {

    }

    std::shared_ptr<void> ChangeFeatureExtraction::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        std::shared_ptr<ChangeObject> changeObjectPtr = std::static_pointer_cast<ChangeObject>(data);
        changeObjectPtr->moving = false;
        if(previousFrame)
        {
            cv::Mat difference = img != *previousFrame;
            cv::erode(difference, difference, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3)), cv::Point(-1,-1), 3);

#ifdef DEBUG
            cv::imshow("Difference", difference);
#endif
            

            changeObjectPtr->nonZero = cv::countNonZero(difference);//(bool)nonzero > 0;
        }
     
        return std::make_shared<cv::Mat>(img);
    }

    void ChangeFeatureExtraction::setPreviousFrame(const cv::Mat& img)
    {
        previousFrame = std::make_shared<cv::Mat>(img);
    }

}