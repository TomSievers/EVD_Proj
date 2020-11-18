#ifndef ICAPTURE_HPP
#define ICAPTURE_HPP

#include <opencv2/core.hpp>
#include <thread>
#include <mutex>
#include <array>

namespace ImageCapture
{
    class ICapture
    {
    public:
        /**
         * @brief set region of interest to given points, resulting transfrom will move the roi into an image the size of width and height.
         * 
         * @param roi Region of interest in camera
         * @param width Image width of the resulting transform
         * @param height Image height of the resulting transform
         */
        virtual void setROI(std::array<cv::Point2f, 4>& roi, cv::Point2f::value_type width, cv::Point2f::value_type height) = 0;
        /**
         * @brief Get the newest frame (transformed if setROI)
         * 
         * @return cv::Mat 
         */
        virtual cv::Mat getFrame() = 0;
        /**
         * @brief Stop getter frame getting thread
         * 
         */
        virtual void stop() = 0;
        /**
         * @brief Update loop called by internal thread
         * 
         */
        virtual void update() = 0;
    protected:
        ICapture(){};
        virtual ~ICapture(){};
        cv::Mat curFrame;
        bool active;
        std::thread thread;
        std::mutex updateMutex;
        cv::Mat transMat;
    }; //ICapture

} // namespace ImageCapture


#endif //ICAPTURE_HPP
