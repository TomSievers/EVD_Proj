#ifndef CHANGE_SEGMENTATION_HPP
#define CHANGE_SEGMENTATION_HPP

#include "include/IImageProcessing.hpp"
#include <opencv2/imgproc.hpp>

namespace Detector
{
    class ChangeSegmentation : public IImageProcessing
    {
        public:
            ChangeSegmentation();
            virtual ~ChangeSegmentation();

            std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);

        private:
            /**
             * @brief function which can be used to remove the pool table background color, to get a image with balls only.
             * 
             * @param image Input image which should be in HSV color format
             * @return cv::Mat A thresholded image where the background color is removed.
             */
            cv::Mat removeBackground(const cv::Mat& image);
    };
}

#endif