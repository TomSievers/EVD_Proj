#include "include/Change/ChangeDetector.hpp"
#include "include/Acquisition.hpp"
#include "include/Change/ChangeEnhancement.hpp"
#include "include/Change/ChangeSegmentation.hpp"
#include "include/Change/ChangeFeatureExtraction.hpp"
#include "include/Change/ChangeClassification.hpp"
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

namespace Detector
{

    ChangeDetector::ChangeDetector(std::shared_ptr<Acquisition> cap) : IDetector(cap)
    {
        processors[ENHANCEMENT] = std::make_shared<ChangeEnhancement>();
        processors[SEGMENTATION] = std::make_shared<ChangeSegmentation>();
        processors[FEATURE_EXTRACT] = std::make_shared<ChangeFeatureExtraction>();
        processors[CLASSIFICATION] = std::make_shared<ChangeClassification>();

        useRoi = false;
    }

    ChangeDetector::~ChangeDetector()
    {

    }

    std::vector<std::shared_ptr<Object>> ChangeDetector::getObjects()
    {
        cv::Mat img;
        std::vector<std::shared_ptr<Object>> objects;
        std::shared_ptr<void> data = std::make_shared<ChangeObject>();
        for(const std::pair<VisionStep, std::shared_ptr<IImageProcessing>>& processor : processors)
        {
            std::shared_ptr<void> returnedData;
            if(processor.first == SEGMENTATION)
            {
                if(useRoi)
                {
                    img = img(cv::Range(roi[0].y, roi[3].y), cv::Range(roi[0].x, roi[1].x));
                }
                else // Remove pockets because of lighting errors
                {
                    img = img(cv::Range(30, img.rows-30), cv::Range(30, img.cols-30));
                }

                returnedData = processor.second->process(img, data);
            }
            else{
                returnedData = processor.second->process(img, data);
            }

            if(returnedData)
            {
                img = *(std::static_pointer_cast<cv::Mat>(returnedData));
            }
        }

        std::shared_ptr<Object> changeObjectPtr = std::static_pointer_cast<Object>(data);
        objects.push_back(changeObjectPtr);

        return objects;
    }

    void ChangeDetector::setRoi(std::array<cv::Point2i, 4>& points)
    {
        roi = points;
        useRoi = true;
    }

    void ChangeDetector::clearRoi()
    {
        useRoi = false;
    }
}