#include <include/Capture.hpp>
#include <opencv2/imgproc.hpp>
#include <functional>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/v4l2-common.h>
#include <linux/v4l2-controls.h>
#include <linux/videodev2.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <map>

namespace ImageCapture
{
    Capture::Capture(int device) : cap(cv::VideoCapture(device)), config("../rpi_hq_cam.cfg")
    {
        if(!cap.isOpened())
        {
            std::string funName = __PRETTY_FUNCTION__;
            throw std::runtime_error(funName + ": Video capture could not be opened");
        }

        std::string dev_str = "/dev/video";
        dev_str += std::to_string(device);

        int fd = open(dev_str.c_str(), O_RDWR);
        if(fd < 0)
        {
            std::string funName = __PRETTY_FUNCTION__;
            throw std::runtime_error(funName + ": Video device could not be opened for settings");
        }

        v4l2_control set;

        for(auto& setting : settings)
        {
            std::string val = config.getValue(setting.name);

            if(!val.empty())
            {
                set.id = setting.id;
                if(!setting.menu_items.empty())
                {
                    for(auto& item : setting.menu_items)
                    {
                        if(item.first.find(val))
                        {
                            set.value = item.second;
                            break;
                        }
                    }
                } else {
                    set.value = std::stoi(val);
                }

                int ret = ioctl(fd, VIDIOC_S_CTRL, &set);
                if(ret != 0)
                {
                    std::cout << "Info: unable to set setting " << setting.name << "; errno: " << errno << std::endl;
                }
            } else 
            {
                std::cout << "Info: " << setting.name << "not found in configuration file" << std::endl;
            }
        }

        int ret = close(fd);
        if(ret != 0)
        {
            std::string funName = __PRETTY_FUNCTION__;
            throw std::runtime_error(funName + ": Video device could not closed be after settings");
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
                    cv::resize(curFrame, curFrame, cv::Size(static_cast<int>(fabs(targetROI[1].x - targetROI[0].x)), static_cast<int>(fabs(targetROI[2].y - targetROI[1].y))));
                    cv::warpPerspective(tmpFrame, curFrame, transMat, curFrame.size());
                }
                updateMutex.unlock();
            }
            update();
        }
    }

    void Capture::setROI(std::array<cv::Point2f, 4>& roi, cv::Point2f::value_type width, cv::Point2f::value_type height)
    {
        targetROI[1].x = width;
        targetROI[2].x = width;
        targetROI[2].y = height;
        targetROI[3].y = height;
        transMat = cv::getPerspectiveTransform(roi, targetROI);
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
