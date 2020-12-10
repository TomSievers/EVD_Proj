#include "include/Ball/BallClassification.hpp"
#include <iostream>

#define MIN_WHITE_PERCENTAGE_CUE_BALL 75

namespace Detector
{
    BallClassification::BallClassification()
    {

    }

    BallClassification::~BallClassification()
    {

    }

    std::shared_ptr<void> BallClassification::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        std::shared_ptr<std::vector<std::shared_ptr<BallObject>>> objectPtr = std::static_pointer_cast<std::vector<std::shared_ptr<BallObject>>>(data);
        classifyBalls(*objectPtr);
        return nullptr;
    }

    void BallClassification::classifyBalls(std::vector<std::shared_ptr<BallObject>>& ballObjects)
    {
        bool found = false;
        uint32_t maxValue = 0;
        std::size_t maxIterator = 0;
        for(std::size_t i = 0; i < ballObjects.size(); ++i)
        {
            if(ballObjects.at(i)->percentageWhite >= MIN_WHITE_PERCENTAGE_CUE_BALL && ballObjects.at(i)->percentageWhite > maxValue)
            {
                maxValue = ballObjects.at(i)->percentageWhite;
                maxIterator = i;
                found = true;
            }
        }

        if(found)
        {
            ballObjects.at(maxIterator)->ballType = CUE_BALL;
        }
    }
}