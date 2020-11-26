#include <include/Acquisition.hpp>
#include <ImageCapture/include/Capture.hpp>

namespace Detector
{
    Acquisition::Acquisition() : cap(std::make_unique<ImageCapture::Capture>(0))
    {

    }

    Acquisition::~Acquisition()
    {
        
    }

    ImageCapture::ICapture& Acquisition::getCapture()
    {
        return *cap;
    }

    std::shared_ptr<void> Acquisition::process(cv::Mat& img)
    {
        img = cap->getFrame();
        return nullptr;
    }



} // namespace Detector
