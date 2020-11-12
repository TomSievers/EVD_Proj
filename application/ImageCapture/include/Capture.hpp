#ifndef CAPTURE_HPP
#define CAPTURE_HPP

#include "ICapture.hpp"
#include "CameraConfig.hpp"
#include <opencv2/videoio.hpp>

namespace ImageCapture
{
    class Capture : public ICapture
    {
    public:
        Capture(int device);
        virtual ~Capture();
        virtual void setROI(std::array<cv::Point, 4>& roi);
        virtual cv::Mat getFrame();
        virtual void stop();
        virtual void update();
    private:
        CameraConfig config;
        cv::VideoCapture cap;
        std::array<cv::Point, 4> targetROI = {cv::Point(0, 0), cv::Point(200, 0), cv::Point(200, 100), cv::Point(0, 100)};
    }; //Capture
} // namespace ImageCapture

#endif //CAPTURE_HPP
