#ifndef BALL_DETECTOR_HPP
#define BALL_DETECTOR_HPP

#include "include/IImageProcessing.hpp"
#include "include/IDetector.hpp"
#include <vector>
#include <memory>

namespace Detector
{
    class BallDetector : public IDetector
    {
        public:
            BallDetector(std::shared_ptr<Acquisition> cap);
            virtual ~BallDetector();

            std::vector<std::shared_ptr<Object>> getObjects();
    };
}

#endif