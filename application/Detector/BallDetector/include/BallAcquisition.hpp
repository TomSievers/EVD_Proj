#ifndef BALL_ACQUISITION_HPP
#define BALL_ACQUISITION_HPP

#include "include/IImageProcessing.hpp"
#include "include/Capture.hpp"
#include <memory>

namespace Detector
{
    class BallAcquisition : public IImageProcessing
    {
        public:
            BallAcquisition();
            virtual ~BallAcquisition();

            /**
             * @brief This function returns an image which the capture component generates
             * 
             * @param image Can be a empty cv::Mat, this parameter is not used.
             * @return cv::Mat Image from the capture class
             */
            cv::Mat getImage(const cv::Mat& image);
            void updateImageData(const cv::Mat& image, std::vector<std::unique_ptr<Object>>& ballObjects);
        private:
            static ImageCapture::Capture capture;
    };
}

#endif