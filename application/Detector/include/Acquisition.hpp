#ifndef ACQUISITION_HPP
#define ACQUISITION_HPP

#include <ImageCapture/include/ICapture.hpp>
#include <include/IImageProcessing.hpp>

namespace Detector
{
    class Acquisition : public IImageProcessing
    {
    public:
        /**
         * @brief Construct a new Acquisition object
         * 
         * @param deviceId id of the camera to use
         */
        Acquisition(int deviceId);
        /**
         * @brief Construct a new Acquisition object
         * 
         * @param mock_img path to a mock image to test underlying detector
         */
        Acquisition(const std::string& mock_img);
        virtual ~Acquisition();
        /**
         * @brief Get the Capture object
         * 
         * @return ImageCapture::ICapture& 
         */
        ImageCapture::ICapture& getCapture();
        /**
         * @brief get newest image from capture and put into img
         * 
         * @param img varaible to put newest image into
         * @return std::shared_ptr<void> nullptr
         */
        virtual std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
    private:
        std::unique_ptr<ImageCapture::ICapture> cap;
    }; //Acquisition

} // namespace Detector

#endif //ACQUISITION_HPP
