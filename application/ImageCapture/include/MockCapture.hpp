#ifndef MOCKCAPTURE_HPP
#define MOCKCAPTURE_HPP

#include "ICapture.hpp"
#include <opencv2/core.hpp>

namespace ImageCapture
{

    class MockCapture : public ICapture
    {
    public:
        MockCapture(const std::string& imgPath);
        virtual ~MockCapture();
        /**
         * @brief set region of interest to given points, resulting transfrom will move the roi into an image the size of width and height.
         * 
         * @param roi Region of interest in camera
         * @param width Image width of the resulting transform
         * @param height Image height of the resulting transform
         */
        virtual void setROI(std::array<cv::Point2f, 4>& roi, cv::Point2f::value_type width, cv::Point2f::value_type height);
        /**
         * @brief Get the newest frame (transformed if setROI)
         * 
         * @return cv::Mat 
         */
        virtual cv::Mat getFrame();
        /**
         * @brief Stop getter frame getting thread
         * 
         */
        virtual void stop();
        
    private:
    /**
         * @brief Update loop called by internal thread
         * 
         */
        virtual void update();
        cv::Mat img;
        std::array<cv::Point2f, 4> targetROI;
    }; //MockCapture

} // namespace ImageCapture

#endif //MOCKCAPTURE_HPP
