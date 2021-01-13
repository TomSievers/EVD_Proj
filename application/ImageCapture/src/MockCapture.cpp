#include <include/MockCapture.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

namespace ImageCapture
{
    MockCapture::MockCapture(const std::string& imgPath) : img(cv::imread(imgPath)), cap(imgPath)
    {
        if(img.empty() && !cap.isOpened())
        {
            std::string funName = __PRETTY_FUNCTION__;
            throw std::runtime_error(funName + ": Mock image/video could not be opened");
        } 
        newFrame = true;
        active.store(true);
        thread = std::thread(&MockCapture::update, this);
    }

    void MockCapture::stop()
    {
        active.store(false);
    }

    void MockCapture::update()
    {
        cv::Mat tmpFrame;
        cv::Mat lastFrame;
        while(active.load())
        {
            auto start = std::chrono::high_resolution_clock::now();
            if(!img.empty())
            {
                tmpFrame = img.clone();
            } else {
                cap >> lastFrame;
                tmpFrame = lastFrame;
                newFrame = false;
            }
            
            if(!tmpFrame.empty())
            {
                updateMutex.lock();
                if(transMat.empty())
                {
                    curFrame = tmpFrame;
                } else {
                    cv::warpPerspective(tmpFrame, curFrame, transMat, cv::Size(static_cast<int>(fabs(targetROI[1].x - targetROI[0].x)), static_cast<int>(fabs(targetROI[2].y - targetROI[1].y))));
                }
                updateMutex.unlock();
            }
            auto end = std::chrono::high_resolution_clock::now();

            auto sleep = std::chrono::microseconds((1000000/30)-std::chrono::duration_cast<std::chrono::microseconds>(end-start).count());
            std::this_thread::sleep_for(sleep);
        }
    }

    void MockCapture::setROI(std::array<cv::Point2f, 4>& roi, cv::Point2f::value_type width, cv::Point2f::value_type height)
    {
        targetROI[0].x = 0;
        targetROI[0].y = 0;
        targetROI[1].x = width;
        targetROI[1].y = 0;
        targetROI[2].x = width;
        targetROI[2].y = height;
        targetROI[3].x = 0;
        targetROI[3].y = height;
        transMat = cv::getPerspectiveTransform(roi, targetROI);
    }

    cv::Mat MockCapture::getFrame()
    {
        std::lock_guard<std::mutex> guard(updateMutex);
        newFrame = true;
        return curFrame;
    }
    

    MockCapture::~MockCapture()
    {
        stop();
        thread.join();
    }
} // namespace ImageCapture
