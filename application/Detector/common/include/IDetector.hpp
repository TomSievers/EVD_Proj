#ifndef IDETECTOR_HPP
#define IDETECTOR_HPP

#include "Object.hpp"
#include <vector>
#include <memory>

namespace Detector
{
    class IDetector
    {
        public:
            IDetector() {};
            virtual ~IDetector() {};
            virtual std::vector<std::unique_ptr<Object>> getObjects() = 0;
    };
}

#endif