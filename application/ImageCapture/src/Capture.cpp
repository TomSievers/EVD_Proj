#include <ImageCapture/include/Capture.hpp>
#include <opencv2/imgproc.hpp>
#include <functional>
#include <iostream>

using namespace std::chrono_literals;

namespace ImageCapture
{
    Capture::Capture(int device) : cap(cv::VideoCapture(device)), config("../test.cfg")
    {
        if(!cap.isOpened())
        {
            std::string funName = __PRETTY_FUNCTION__;
            throw std::runtime_error(funName + ": Video capture could not be opened");
        }
        active = true;
        thread = std::thread(&Capture::update, this);
    }

    void Capture::stop()
    {
        std::lock_guard<std::mutex> guard(updateMutex);
        active = false;
    }

    void Capture::update()
    {
        cv::Mat tmpFrame;
        if(active)
        {
            cap >> tmpFrame;

            if(!tmpFrame.empty())
            {
                updateMutex.lock();
                if(transMat.empty())
                {
                    curFrame = tmpFrame;
                } else {
                    cv::resize(curFrame, curFrame, cv::Size(200, 100));
                    cv::warpAffine(tmpFrame, curFrame, transMat, curFrame.size());
                }
                updateMutex.unlock();
            }
            update();
        }
    }

    void Capture::setROI(std::array<cv::Point, 4>& roi)
    {
        transMat = cv::getAffineTransform(roi, targetROI);
    }

    cv::Mat Capture::getFrame()
    {
        std::lock_guard<std::mutex> guard(updateMutex);
        return curFrame;
    }
    

    Capture::~Capture()
    {
        stop();
        thread.join();
    }
} // namespace ImageCapture
