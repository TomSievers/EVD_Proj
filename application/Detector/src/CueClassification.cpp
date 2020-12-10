#include <include/IDetector.hpp>
#include <include/Cue/CueProcessing.hpp>
#include <iostream>
namespace Detector
{
    CueClassification::CueClassification()
    {

    }

    CueClassification::~CueClassification()
    {

    }

    std::shared_ptr<void> CueClassification::process(cv::Mat& image, std::shared_ptr<void> data)
    {
        if(data != nullptr)
        {
            (void) image;
            std::shared_ptr<CueObject> cue = std::make_shared<CueObject>();
            std::vector<cv::Point> cornerPoints = *std::static_pointer_cast<std::vector<cv::Point>>(data);
            cv::Point center = classifyCue(cornerPoints);
            cue->center = center;
            std::cout << "test" << std::endl;
            return cue;
        }
        return nullptr;
    }

    cv::Point CueClassification::classifyCue(const std::vector<cv::Point> &cornerPoints)
    {
        uint64_t totalX = 0, totalY = 0;
        for(uint8_t i = 0; i < cornerPoints.size(); i++)
        {
            totalX += cornerPoints[i].x;
            totalY += cornerPoints[i].y;
        }
        return cv::Point((int)(totalX/cornerPoints.size()), (int)(totalY/cornerPoints.size()));
    }
}
