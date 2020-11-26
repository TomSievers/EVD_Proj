#ifndef ACQUISITION_HPP
#define ACQUISITION_HPP

#include <ImageCapture/include/ICapture.hpp>
#include <include/IImageProcessing.hpp>

namespace Detector
{
    class Acquisition : public IImageProcessing
    {
    public:
        Acquisition();
        virtual ~Acquisition();
        ImageCapture::ICapture& getCapture();
        virtual std::shared_ptr<void> process(cv::Mat& img);
    private:
        std::unique_ptr<ImageCapture::ICapture> cap;
    }; //Acquisition

} // namespace Detector

#endif //ACQUISITION_HPP
