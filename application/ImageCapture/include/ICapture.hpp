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
        virtual void setROI(std::array<cv::Point, 4>& roi) = 0;
        virtual cv::Mat getFrame() = 0;
        virtual void stop() = 0;
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
