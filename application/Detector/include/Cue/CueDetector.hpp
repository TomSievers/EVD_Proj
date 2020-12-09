#ifndef CUEDETECTOR_HPP
#define CUEDETECTOR_HPP

#include <include/IDetector.hpp>
#include <include/IImageProcessing.hpp>

namespace Detector
{
    class CueDetector : public IDetector
    {
    public:
        CueDetector(std::shared_ptr<Acquisition> cap);
        virtual ~CueDetector();
        std::vector<std::shared_ptr<Object>> getObjects();
    private:
        cv::Mat img;
    };
} //namespace Detector
#endif