#include <ImageCapture/include/Capture.hpp>
#include <opencv2/highgui.hpp>
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
#include <sys/mman.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    ImageCapture::Capture cap(2);

    std::array<cv::Point2f, 4> roi = {cv::Point2f(10, 100), cv::Point2f(200, 100), cv::Point2f(200, 200), cv::Point2f(10, 200)};

    while(true)
    {
        cv::Mat frame = cap.getFrame();
        if(!frame.empty())
        {
            cv::imshow("test", frame);
        }

        int key = cv::waitKey(1);

        if(key == 27)
        {
            cap.stop();
            break;
        } else if(key == 114) {
            cap.setROI(roi, 400, 200);
        }
    }

    
    return 0;
}
