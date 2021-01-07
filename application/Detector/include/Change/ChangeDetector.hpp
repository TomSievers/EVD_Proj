#ifndef CHANGE_DETECTOR_HPP
#define CHANGE_DETECTOR_HPP

#include "include/IImageProcessing.hpp"
#include "include/IDetector.hpp"
#include <vector>
#include <memory>

namespace Detector
{
    class ChangeDetector : public IDetector
    {
        public:
            ChangeDetector(std::shared_ptr<Acquisition> cap);
            virtual ~ChangeDetector();

            std::vector<std::shared_ptr<Object>> getObjects();

        private:
            static cv::Mat frame; 
    };
}

#endif