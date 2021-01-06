#ifndef CUE_CONF_DETECTOR_HPP
#define CUE_CONF_DETECTOR_HPP

#include "include/IImageProcessing.hpp"
#include "include/IDetector.hpp"
#include "include/Acquisition.hpp"

namespace Detector
{
    class CueConfDetector : public IDetector
    {
        public:
            CueConfDetector(std::shared_ptr<Acquisition> cap);
            virtual ~CueConfDetector();

            void setCurrentConfig(std::shared_ptr<Object> aConfig);
            std::vector<std::shared_ptr<Object>> getObjects();
        private:
            std::shared_ptr<Config> configPtr;
    };
}

#endif