#ifndef BALL_DETECTOR_HPP
#define BALL_DETECTOR_HPP

#include "include/Object.hpp"
#include "include/IImageProcessing.hpp"
#include "include/IDetector.hpp"
#include <vector>
#include <memory>

namespace Detector
{
    class BallDetector : public IDetector
    {
        public:
            BallDetector();
            virtual ~BallDetector();

            std::vector<std::unique_ptr<Object>> getObjects();
        private:
            std::vector<std::unique_ptr<IImageProcessing>> imageProcessors;
    };
}

#endif