#include "cuedetector.hpp"

namespace Detector
{
    const int max_value_H = 360/2;
    const int max_value = 255;
    const std::string sliders = "Sliders";
    int low_H = 72, low_S = 58, low_V = 78;
    int high_H = 105, high_S = 255, high_V = 255;

    static void on_low_H_thresh_trackbar(int, void *)
    {
        low_H = cv::min(high_H-1, low_H);
        cv::setTrackbarPos("Low H", sliders, low_H);
    }
    static void on_high_H_thresh_trackbar(int, void *)
    {
        high_H = cv::max(high_H, low_H+1);
        cv::setTrackbarPos("High H", sliders, high_H);
    }
    static void on_low_S_thresh_trackbar(int, void *)
    {
        low_S = cv::min(high_S-1, low_S);
        cv::setTrackbarPos("Low S", sliders, low_S);
    }
    static void on_high_S_thresh_trackbar(int, void *)
    {
        high_S = cv::max(high_S, low_S+1);
        cv::setTrackbarPos("High S", sliders, high_S);
    }
    static void on_low_V_thresh_trackbar(int, void *)
    {
        low_V = cv::min(high_V-1, low_V);
        cv::setTrackbarPos("Low V", sliders, low_V);
    }
    static void on_high_V_thresh_trackbar(int, void *)
    {
        high_V = cv::max(high_V, low_V+1);
        cv::setTrackbarPos("High V", sliders, high_V);
    }

    CueDetector::CueDetector()
    {

    }

    CueDetector::~CueDetector()
    {

    }

    void CueDetector::run()
    {
        cv::namedWindow(sliders);
        cv::createTrackbar("Low H", sliders, &low_H, max_value_H, on_low_H_thresh_trackbar);
        cv::createTrackbar("High H", sliders, &high_H, max_value_H, on_high_H_thresh_trackbar);
        cv::createTrackbar("Low S", sliders, &low_S, max_value, on_low_S_thresh_trackbar);
        cv::createTrackbar("High S", sliders, &high_S, max_value, on_high_S_thresh_trackbar);
        cv::createTrackbar("Low V", sliders, &low_V, max_value, on_low_V_thresh_trackbar);
        cv::createTrackbar("High V", sliders, &high_V, max_value, on_high_V_thresh_trackbar);


        src = imread("D:/EVD_Proj/Photos_pool_table/setup4_3.jpg",cv::IMREAD_COLOR);
        cv::GaussianBlur(src, src, cv::Size(7,7), 10, 0);


        while(true)
        {
            src.copyTo(copy_src);
            cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);
            cv::inRange(hsv, cv::Scalar(low_H, low_S, low_V), cv::Scalar(high_H, high_S, high_V), threshold);

            std::vector<std::vector<cv::Point>> contours, contours0;
            std::vector<cv::Vec4i> hierarchy;
            cv::findContours(threshold,contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
            cv::Mat drawing = cv::Mat::zeros(threshold.size(), CV_8UC3);
            contours0.resize(contours.size());

            for(std::size_t i = 0; i < contours.size(); i++)
            {
                cv::drawContours(drawing, contours, (int)i, cv::Scalar(255,255,255), 2, cv::LINE_8, hierarchy, 0);
                double epsilon = cv::arcLength(contours[i], true);
                cv::approxPolyDP(contours[i], contours0[i], epsilon*0.03, true);
            }

            for(std::size_t i = 0; i < contours0.size(); i++)
            {
                std::cout << contours0[i].size() << std::endl;
                if(contours0[i].size() == 4 || contours0[i].size() == 5)
                {
                    cv::drawContours(copy_src, contours0, i, cv::Scalar(255,0,0),2);
                }
            }

            cv::imshow("threshold", threshold);
            cv::imshow("contours", drawing);
            cv::imshow("src", src);
            cv::imshow("copy_src", copy_src);
            char key = (char)cv::waitKey(30);
            if(key == 'q')
            {
                break;
            }
        }
    }

}//Detector
