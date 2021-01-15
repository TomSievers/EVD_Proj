#include "include/Configure/CueConfDetector.hpp"
#include "include/Configure/CueConfEnhancement.hpp"
#include "include/Configure/CueConfFeatureExtraction.hpp"
#include "include/Configure/CueConfSegmentation.hpp"
#include <chrono>
#include <memory>
#include <iostream>

namespace Detector
{
    CueConfDetector::CueConfDetector(std::shared_ptr<Acquisition> cap) : IDetector(cap)
    {
        processors[ENHANCEMENT] = std::make_shared<CueConfEnhancement>();
        processors[SEGMENTATION] = std::make_shared<CueConfSegmentation>();
        processors[FEATURE_EXTRACT] = std::make_shared<CueConfFeatureExtraction>();
    }

    CueConfDetector::~CueConfDetector()
    {

    }

    void CueConfDetector::setCurrentConfig(std::shared_ptr<Object> aConfig)
    {
        configPtr = std::dynamic_pointer_cast<Config>(aConfig);
    }

    std::vector<std::shared_ptr<Object>> CueConfDetector::getObjects()
    {
        std::vector<std::shared_ptr<Object>> objects;
        if(configPtr)
        {
            std::array<uint64_t, 3> hsvMinSum = {0, 0, 0};
            std::array<uint64_t, 3> hsvMaxSum = {0, 0, 0};
            uint32_t numberOfRuns = 0;

            std::chrono::system_clock::time_point stamp = std::chrono::system_clock::now();
            std::chrono::duration<int> duration(2);

            while(std::chrono::system_clock::now() < (stamp + duration))
            {
                cv::Mat img;
                std::shared_ptr<Config> currentConfigPtr = std::make_shared<Config>();
                currentConfigPtr->roiPositions = configPtr->roiPositions;
                currentConfigPtr->tableColorMin = configPtr->tableColorMin;
                currentConfigPtr->tableColorMax = configPtr->tableColorMax;
                currentConfigPtr->cueBallColorMin = configPtr->cueBallColorMin;
                currentConfigPtr->cueBallColorMax = configPtr->cueBallColorMax;

                std::shared_ptr<void> data = currentConfigPtr;

                bool continueToNextLoop = false;

                for(const std::pair<VisionStep, std::shared_ptr<IImageProcessing>>& processor : processors)
                {
                    processor.second->process(img, data);
                    if(img.empty())
                    {
                        continueToNextLoop = true;
                        break;
                    }
                }

                if(continueToNextLoop)
                    continue;

                for(std::size_t i = 0; i < hsvMinSum.size(); ++i)
                {
                    hsvMinSum[i] += currentConfigPtr->cueColorMin[i];
                    hsvMaxSum[i] += currentConfigPtr->cueColorMax[i];
                }
                numberOfRuns++;
            }

            if(numberOfRuns > 0)
            {
                for(std::size_t i = 0; i < hsvMinSum.size(); ++i)
                {
                    configPtr->cueColorMin[i] = (uint8_t) (hsvMinSum[i] / numberOfRuns) - 10;
                    configPtr->cueColorMax[i] = (uint8_t) (hsvMaxSum[i] / numberOfRuns);
                    if (i != 0) // dont add extra to the max range of hue
                        configPtr->cueColorMax[i] += 10;
                }

                objects.push_back(configPtr);
            }
        }
        return objects;
    }
}