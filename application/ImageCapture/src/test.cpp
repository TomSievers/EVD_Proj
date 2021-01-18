#include <include/Capture.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

int main(int argc, char const *argv[])
{
    ImageCapture::Capture cap(0);

    std::array<cv::Point2f, 4> roi = {cv::Point2f(10, 100), cv::Point2f(200, 100), cv::Point2f(200, 200), cv::Point2f(10, 200)};

    std::vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);
    int i = 0;

    while(true)
    {
        cv::Mat frame = cap.getFrame();
        if(!frame.empty() && frame.cols != 0 && frame.rows != 0)
        {
                    cv::imwrite("testFrame" + std::to_string(i) + ".png", frame, compression_params);
                    ++i;
                    if(i == 100)
            {
                break;
            }

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
