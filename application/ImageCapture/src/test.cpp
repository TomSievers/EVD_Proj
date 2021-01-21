#include <include/Capture.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

int main(int argc, char const *argv[])
{
    ImageCapture::Capture cap(0);

    /*std::array<cv::Point2f, 4> roi = {cv::Point2f(10, 100), cv::Point2f(200, 100), cv::Point2f(200, 200), cv::Point2f(10, 200)};
    int i = 0;
    while(true)
    {
        cv::Mat frame = cap.getFrame();
        if(!frame.empty() && frame.cols != 0 && frame.rows != 0)
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
        
    }*/

    int hue_min = 100;
    int sat_min = 240;
    int val_min = 90;
    int hue_max = 117;
    int sat_max = 255;
    int val_max = 255;

    cv::namedWindow("sliders");

    cv::createTrackbar("hue_min", "sliders", &hue_min, 180);
    cv::createTrackbar("hue_max", "sliders", &hue_max, 180);
    cv::createTrackbar("sat_min", "sliders", &sat_min, 255);
    cv::createTrackbar("sat_max", "sliders", &sat_max, 255);
    cv::createTrackbar("val_min", "sliders", &val_min, 255);
    cv::createTrackbar("val_max", "sliders", &val_max, 255);

    cv::Mat mask;

    cv::Mat mask2;

    cv::Mat res;

    while(true)
    {
        cv::Mat img = cap.getFrame();

        if(!img.empty())
        {
            cv::cvtColor(img, img, cv::COLOR_BGR2BGRA);

            cv::Mat hsv;

            cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);

            cv::inRange(hsv, cv::Scalar(hue_min, sat_min, val_min), cv::Scalar(hue_max, sat_max, val_max), mask);

            cv::cvtColor(mask, mask2, cv::COLOR_GRAY2BGRA);

            cv::addWeighted(img, 0.6, mask2, 0.2, 0, res);

            cv::imshow("res", res);

            if(cv::waitKey(1) == 27)
            {
                break;
            }
        }
    }

    /*int hue_min = 100;
    int hue_max = 255;

    cv::Mat mask;

    cv::Mat mask2;

    cv::Mat gray;

    cv::Mat res;

    cv::namedWindow("sliders");

    cv::createTrackbar("hue_min", "sliders", &hue_min, hue_max);
    cv::createTrackbar("hue_max", "sliders", &hue_max, hue_max);

    while(true)
    {
        cv::Mat img = cap.getFrame();

        if(!img.empty())
        {
            cv::cvtColor(img, img, cv::COLOR_BGR2BGRA);

            cv::cvtColor(img, gray, cv::COLOR_BGRA2GRAY);
            
            cv::inRange(gray, cv::Scalar(hue_min), cv::Scalar(hue_max), mask);

            cv::cvtColor(mask, mask2, cv::COLOR_GRAY2BGRA);

            cv::addWeighted(img, 0.6, mask2, 0.2, 0, res);

            cv::imshow("res", res);

            if(cv::waitKey(1) == 27)
            {
                break;
            }
        }
    }*/
    return 0;
}
