#include "include/Configure/TableConfDetector.hpp"
#include "include/Configure/TableConfEnhancement.hpp"
#include "include/Configure/TableConfSegmentation.hpp"
#include "include/Configure/TableConfFeatureExtraction.hpp"
#include <chrono>
#include <memory>
#include <iostream>

namespace Detector
{
    TableConfDetector::TableConfDetector(std::shared_ptr<Acquisition> cap) : IDetector(cap)
    {
        processors[ENHANCEMENT] = std::make_shared<TableConfEnhancement>();
        processors[SEGMENTATION] = std::make_shared<TableConfSegmentation>();
        processors[FEATURE_EXTRACT] = std::make_shared<TableConfFeatureExtraction>();
    }

    TableConfDetector::~TableConfDetector()
    {

    }

    std::vector<std::shared_ptr<Object>> TableConfDetector::getObjects()
    {
        std::vector<std::shared_ptr<Object>> objects;
        std::shared_ptr<Config> configPtr = std::make_shared<Config>();
        std::array<uint64_t, 3> hsvMinSum = {0, 0, 0};
        std::array<uint64_t, 3> hsvMaxSum = {0, 0, 0};
        uint32_t numberOfRuns = 0;

        std::chrono::system_clock::time_point stamp = std::chrono::system_clock::now();
        std::chrono::duration<int> duration(4);

        // for duration loop through new images and determine the avg pixel color of the table
        while(std::chrono::system_clock::now() < (stamp + duration))
        {
            cv::Mat img;
            std::shared_ptr<void> data = std::make_shared<Config>();
            
            bool continueToNextLoop = false;

            for(const std::pair<VisionStep, std::shared_ptr<IImageProcessing>>& processor : processors)
            {
                processor.second->process(img, data);
                if(img.empty()) // in case no region in interest can be found, continue
                {
                    continueToNextLoop = true;
                    break;;
                }
            }

            if(continueToNextLoop)
                continue;

            std::shared_ptr<Config> currentConfigPtr = std::static_pointer_cast<Config>(data);
            
            configPtr->roiPositions = currentConfigPtr->roiPositions; 
            for(std::size_t i = 0; i < hsvMinSum.size(); ++i)
            {
                hsvMinSum[i] += currentConfigPtr->tableColorMin[i];
                hsvMaxSum[i] += currentConfigPtr->tableColorMax[i];
            }
            numberOfRuns++;
        }

        if(numberOfRuns != 0)
        {
            for(std::size_t i = 0; i < hsvMinSum.size(); ++i)
            {
                configPtr->tableColorMin[i] = (uint8_t) (hsvMinSum[i] / numberOfRuns);
                configPtr->tableColorMax[i] = (uint8_t) (hsvMaxSum[i] / numberOfRuns);
            }

            // TODO: change this so we can work with a small range like -20 +20
            configPtr->tableColorMin[0] -= 10;
            configPtr->tableColorMax[0] += 10;
            configPtr->tableColorMin[1] = 0;
            configPtr->tableColorMax[1] = 255;
            configPtr->tableColorMin[2] -= 10;
            configPtr->tableColorMax[2] += 30;

            objects.push_back(configPtr);
        }
        return objects;
    }
}