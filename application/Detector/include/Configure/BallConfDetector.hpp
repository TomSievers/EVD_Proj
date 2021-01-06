#ifndef BALL_CONF_DETECTOR_HPP
#define BALL_CONF_DETECTOR_HPP

#include "include/IImageProcessing.hpp"
#include "include/IDetector.hpp"
#include "include/Acquisition.hpp"

namespace Detector
{
    class BallConfDetector : public IDetector
    {
        public:
            BallConfDetector(std::shared_ptr<Acquisition> cap);
            virtual ~BallConfDetector();

            void setCurrentConfig(std::shared_ptr<Object> aConfig);
            /**
             * @brief This function returns the found average min and max pixel value for the cue ball
             * It will run over several images to determine the average
             * 
             * @return std::vector<std::shared_ptr<Object>> A struct which contains the min and max hsv values for the ball
             */
            std::vector<std::shared_ptr<Object>> getObjects();
        private:
            std::shared_ptr<Config> configPtr;
    };
}

#endif