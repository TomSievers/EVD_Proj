#ifndef CUEDETECTOR_HPP
#define CUEDETECTOR_HPP
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "iostream"
#include <string>
namespace Detector
{

    class CueDetector
    {
    public:
        CueDetector();
        ~CueDetector();
        void run();
    private:
        cv::Mat src;
        cv::Mat hsv;
        cv::Mat threshold;
        cv::Mat copy_src;
    };
}
#endif // CUEDETECTOR_HPP
