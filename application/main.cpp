#include <ImageCapture/include/Capture.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std::chrono_literals;

int main(int argc, char const *argv[])
{
    ImageCapture::Capture cap(2);

    std::this_thread::sleep_for(1s);

    while(true)
    {
        cv::Mat frame = cap.getFrame();

        cv::imshow("test", frame);

        if(cv::waitKey(1) == 27)
        {
            cap.stop();
            break;
        }
    }

    
    return 0;
}
