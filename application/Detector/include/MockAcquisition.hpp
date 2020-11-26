#ifndef MOCKACQUISITION_HPP
#define MOCKACQUISITION_HPP

#include <ImageCapture/include/ICapture.hpp>
#include <include/IImageProcessing.hpp>

namespace Detector
{
    class MockAcquisition : public IImageProcessing
    {
    public:
        MockAcquisition();
        virtual ~MockAcquisition();
        virtual std::shared_ptr<void> process(cv::Mat& img);
    private:
        cv::Mat img;
    }; //Acquisition

} // namespace Detector

#endif //MOCKACQUISITION_HPP
