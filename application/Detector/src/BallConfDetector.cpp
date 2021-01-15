#include "include/Configure/BallConfDetector.hpp"
#include "include/Configure/BallConfEnhancement.hpp"
#include "include/Configure/BallConfFeatureExtraction.hpp"
#include "include/Configure/BallConfSegmentation.hpp"
#include "include/Acquisition.hpp"
#include <iostream>
#include <chrono>

namespace Detector
{
    BallConfDetector::BallConfDetector(std::shared_ptr<Acquisition> cap) : IDetector(cap)
    {
        processors[ENHANCEMENT] = std::make_shared<BallConfEnhancement>();
        processors[SEGMENTATION] = std::make_shared<BallConfSegmentation>();
        processors[FEATURE_EXTRACT] = std::make_shared<BallConfFeatureExtraction>();
    }

    BallConfDetector::~BallConfDetector()
    {

    }

    void BallConfDetector::setCurrentConfig(std::shared_ptr<Object> aConfig)
    {
        configPtr = std::dynamic_pointer_cast<Config>(aConfig);
    }

    std::vector<std::shared_ptr<Object>> BallConfDetector::getObjects()
    {
        std::vector<std::shared_ptr<Object>> objects;
        if(configPtr)
        {
            std::array<uint64_t, 3> hsvMinSum = {0, 0, 0};
            std::array<uint64_t, 3> hsvMaxSum = {0, 0, 0};
            uint32_t numberOfRuns = 0;

            std::chrono::system_clock::time_point stamp = std::chrono::system_clock::now();
            std::chrono::duration<int> duration(2);

            // for duration loop through new images and determine the avg pixel color of the table
            while(std::chrono::system_clock::now() < (stamp + duration))
            {
                cv::Mat img;
                std::shared_ptr<Config> currentConfigPtr = std::make_shared<Config>();
                currentConfigPtr->roiPositions = configPtr->roiPositions;
                currentConfigPtr->tableColorMin = configPtr->tableColorMin;
                currentConfigPtr->tableColorMax = configPtr->tableColorMax;

                std::shared_ptr<void> data = currentConfigPtr;

                for(const std::pair<VisionStep, std::shared_ptr<IImageProcessing>>& processor : processors)
                {
                    processor.second->process(img, data);
                }

                for(std::size_t i = 0; i < hsvMinSum.size(); ++i)
                {
                    hsvMinSum[i] += currentConfigPtr->cueBallColorMin[i];
                    hsvMaxSum[i] += currentConfigPtr->cueBallColorMax[i];
                }
                numberOfRuns++;
            }

            for(std::size_t i = 0; i < hsvMinSum.size(); ++i)
            {
                configPtr->cueBallColorMin[i] = (uint8_t) (hsvMinSum[i] / numberOfRuns) - 8;
                configPtr->cueBallColorMax[i] = (uint8_t) (hsvMaxSum[i] / numberOfRuns) + 8;
            }

            objects.push_back(configPtr);
        }
        return objects;
    }
}