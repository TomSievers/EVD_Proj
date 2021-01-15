#include "include/Configure/CueConfSegmentation.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <time.h>

cv::Mat image;
cv::Mat cannyImage;
/*
int a = 0;
int b = 255;

void on_trackbar(int, void*)
{
    cv::Canny(image, cannyImage, a, b);
    cv::imshow("image", cannyImage);
}*/

namespace Detector
{
    CueConfSegmentation::CueConfSegmentation()
    {

    }

    CueConfSegmentation::~CueConfSegmentation()
    {

    }

    std::shared_ptr<void> CueConfSegmentation::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        std::shared_ptr<Config> dataPtr = std::static_pointer_cast<Config>(data);
        cv::Mat ROI = img(cv::Rect(dataPtr->roiPositions[0], dataPtr->roiPositions[2], dataPtr->roiPositions[1] - dataPtr->roiPositions[0], dataPtr->roiPositions[3] - dataPtr->roiPositions[2]));
        cv::Mat imageWithoutGrip = removeCueGrip(ROI, *dataPtr);
        imageWithoutGrip.copyTo(img);
        return nullptr;
    }

    cv::Mat CueConfSegmentation::removeCueGrip(cv::Mat& img, Config& config)
    {
        cv::Mat gray;
        cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
        
        cv::Mat resizedImage;
        cv::resize(gray, resizedImage, cv::Size(), 2, 2);

        cv::Mat canny;
        cv::Canny(resizedImage, canny, 0, 25);

        cv::Mat dilated;
        cv::Mat eroded;
        cv::dilate(canny, dilated, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
        cv::erode(dilated, eroded, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
        
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(eroded, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);

        cv::Mat cueEndPoint;

        // there should be 3 contours, a cue grip and the cue end point and the external contour around the cue
        if(contours.size() == 3)
        {
            // remove the parent contour
            for(std::size_t i = 0; i < hierarchy.size(); ++i)
            {
                if(hierarchy.at(i)[3] == -1)
                {
                    contours.erase(contours.begin() + i);
                }
            }

            double minDistance = resizedImage.rows * resizedImage.cols;
            int contourMinDistance = -1;

            for(std::size_t i = 0; i < contours.size(); ++i)
            {
                for(std::size_t k = 0; k < contours.at(i).size(); ++k)
                {

                    double distance = 0;
                    if((distance = distancePointAndLine(cv::Point(0, 0), cv::Point(resizedImage.cols-1, 0), contours.at(i).at(k))) < minDistance ||
                    (distance = distancePointAndLine(cv::Point(resizedImage.cols-1, 0), cv::Point(resizedImage.cols-1, resizedImage.rows-1), contours.at(i).at(k))) < minDistance ||
                    (distance = distancePointAndLine(cv::Point(resizedImage.cols-1, resizedImage.rows-1), cv::Point(0, resizedImage.rows-1), contours.at(i).at(k))) < minDistance ||
                    (distance = distancePointAndLine(cv::Point(0, resizedImage.rows-1), cv::Point(0, 0), contours.at(i).at(k))) < minDistance)
                    {
                        minDistance = distance;
                        contourMinDistance = (int) i;
                    }
                }
            }

            if(contourMinDistance != -1)
            {
                cv::cvtColor(img, cueEndPoint, cv::COLOR_BGR2HSV);
                
                int contourToKeep = (contourMinDistance == 1 ? contourToKeep = 0 : contourToKeep = 1);

                cv::Mat maskContourToKeep = cv::Mat::zeros(resizedImage.rows, resizedImage.cols, resizedImage.type());
                cv::drawContours(maskContourToKeep, contours, contourToKeep, 255, -1);

                cv::Mat eroded;
                cv::erode(maskContourToKeep, eroded, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5)));

                cv::Mat resizedMask;
                cv::resize(eroded, resizedMask, cv::Size(), 0.5, 0.5);

                for(std::size_t i = 0; i < resizedMask.rows; ++i)
                {
                    for(std::size_t k = 0; k < resizedMask.cols; ++k)
                    {
                        if(resizedMask.at<uint8_t>(i, k) == 0)
                        {
                            cueEndPoint.at<cv::Vec3b>(i, k)[0] = 0;
                            cueEndPoint.at<cv::Vec3b>(i, k)[1] = 0;
                            cueEndPoint.at<cv::Vec3b>(i, k)[2] = 0;
                        }
                    }
                }
            } 
        }
        else
        {
            return cv::Mat();
        }

        return cueEndPoint;
    }

    void CueConfSegmentation::determineLine(const cv::Point& point1, const cv::Point& point2, uint16_t& a, uint16_t& b, uint16_t& c)
    {
        a = point1.y - point2.y;
        b = point2.x - point1.x;
        c = point1.x * point2.y - point2.x * point1.y;
    }
    
    double CueConfSegmentation::distancePointAndLine(const cv::Point& point1, const cv::Point& point2, const cv::Point& distancePoint)
    {
        uint16_t a, b, c;
        determineLine(point1, point2, a, b, c);
        return abs(a * distancePoint.x + b * distancePoint.y + c) / sqrt(a * a + b * b);
    }
}