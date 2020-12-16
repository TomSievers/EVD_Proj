#include <include/IDetector.hpp>
#include <include/Cue/CueDetector.hpp>
#include <include/Cue/CueProcessing.hpp>
#include <iostream>
#include "opencv2/highgui.hpp"

namespace Detector
{
CueDetector::CueDetector(std::shared_ptr<Acquisition> cap) : IDetector(cap)
{
    processors[ENHANCEMENT] = std::make_shared<CueEnhancement>();
    processors[SEGMENTATION] = std::make_shared<CueSegmentation>();
    processors[FEATURE_EXTRACT] = std::make_shared<CueFeatureExtraction>();
    processors[CLASSIFICATION] = std::make_shared<CueClassification>();
}

CueDetector::~CueDetector()
{

}

std::vector<std::shared_ptr<Object>> CueDetector::getObjects()
{
#ifdef DEBUG
    std::string names[5] = {"acquisition", "ehance", "segment", "extract", "classify"};
    int i = 0;
#endif

    std::vector<std::shared_ptr<Object>> cues;
    std::shared_ptr<void> data = nullptr;
    cv::Mat copy;

    for(auto& processor : processors)
    {
        
        auto data_ptr = processor.second->process(img, data);
        data = nullptr;
if(processor.first == ACQUISITION)
        {
            img.copyTo(copy);
            cv::imshow("copy", copy);
        }
#ifdef DEBUG
        cv::namedWindow(names[i], cv::WINDOW_KEEPRATIO);
        cv::imshow(names[i], img);

        ++i;
#endif

        if(data_ptr != nullptr)
        {
            switch (processor.first)
            {
            case SEGMENTATION:
            {
                std::vector<std::vector<cv::Point>> contours = *std::static_pointer_cast<std::vector<std::vector<cv::Point>>>(data_ptr);
                data = std::make_shared<std::vector<std::vector<cv::Point>>>(contours);
                break;
            }
            case FEATURE_EXTRACT:
            {
                std::vector<cv::Point> cornerPoints = *std::static_pointer_cast<std::vector<cv::Point>>(data_ptr);
                data = std::make_shared<cueClassificationData>(cornerPoints, copy);
                break;
            }

            case CLASSIFICATION:
            {
                cues.push_back(std::static_pointer_cast<Object>(data_ptr));
                break;
            }
            default:
            {
                throw(std::runtime_error("Unexpected data pointer returned by processor"));
                break;
            }
            }
        }
    }
    return cues;
}
}//namespace Detector
