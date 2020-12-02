#include <include/Acquisition.hpp>
#include <ImageCapture/include/Capture.hpp>
#include <ImageCapture/include/MockCapture.hpp>
#include <iostream>

namespace Detector
{
    Acquisition::Acquisition(int deviceId) : cap(std::make_unique<ImageCapture::Capture>(deviceId))
    {

    }

    Acquisition::Acquisition(const std::string& mock_img) : cap(std::make_unique<ImageCapture::MockCapture>(mock_img))
    {

    }

    Acquisition::~Acquisition()
    {
        
    }

    ImageCapture::ICapture& Acquisition::getCapture()
    {
        return *cap;
    }

    std::shared_ptr<void> Acquisition::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        do
        {
            img = cap->getFrame();
        } while (img.empty());
        return nullptr;
    }



} // namespace Detector
