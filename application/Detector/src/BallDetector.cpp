#include "include/Ball/BallDetector.hpp"
#include "include/Acquisition.hpp"
#include "include/Ball/BallEnhancement.hpp"
#include "include/Ball/BallSegmentation.hpp"
#include "include/Ball/BallFeatureExtraction.hpp"
#include "include/Ball/BallClassification.hpp"
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

namespace Detector
{
    BallDetector::BallDetector(std::shared_ptr<Acquisition> cap) : IDetector(cap)
    {
        processors[ENHANCEMENT] = std::make_shared<BallEnhancement>();
        processors[SEGMENTATION] = std::make_shared<BallSegmentation>();
        processors[FEATURE_EXTRACT] = std::make_shared<BallFeatureExtraction>();
        processors[CLASSIFICATION] = std::make_shared<BallClassification>();
    }

    BallDetector::~BallDetector()
    {

    }

    std::vector<std::shared_ptr<Object>> BallDetector::getObjects()
    {
        std::vector<std::shared_ptr<Object>> objects;
        cv::Mat EnhancedImage;
        cv::Mat img;
        std::shared_ptr<void> data = std::make_shared<std::vector<std::shared_ptr<BallObject>>>();

        for(const std::pair<VisionStep, std::shared_ptr<IImageProcessing>>& processor : processors)
        {
            // set the enhanced image for feature extraction
            if(processor.first == FEATURE_EXTRACT)
            {
                std::dynamic_pointer_cast<BallFeatureExtraction>(processor.second)->setEnhancementImage(EnhancedImage);
            }

            std::shared_ptr<void> returnedData = processor.second->process(img, data);

            if(returnedData)
            {
                img = *(std::static_pointer_cast<cv::Mat>(returnedData));

                if(processor.first == ENHANCEMENT)
                {
                    EnhancedImage = img;
                }
            }

#ifdef DEBUG
            if(!img.empty())
            {
                cv::imshow("window", img);
            }
#endif
        }

        std::shared_ptr<std::vector<std::shared_ptr<BallObject>>> ballObjectPtr = std::static_pointer_cast<std::vector<std::shared_ptr<BallObject>>>(data);
        for(std::size_t i = 0; i < ballObjectPtr->size(); ++i)
        {
            objects.push_back(ballObjectPtr->at(i));
        }

#ifdef DEBUG
        for(std::size_t i = 0; i < objects.size(); ++i)
        {
            if(!ballObjectPtr->at(i)->ballType) // no cue ball
                cv::circle(EnhancedImage, ballObjectPtr->at(i)->point, ballObjectPtr->at(i)->radius, cv::Scalar(255, 0, 0), 1);
            else
                cv::circle(EnhancedImage, ballObjectPtr->at(i)->point, ballObjectPtr->at(i)->radius, cv::Scalar(0, 0, 255), -1);
        }
        cv::imshow("results", EnhancedImage);
#endif

        return objects;
    }
}