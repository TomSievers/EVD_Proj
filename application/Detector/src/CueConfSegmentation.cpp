#include "include/Configure/CueConfSegmentation.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

cv::Mat image;
cv::Mat cannyImage;

//int a = 0;
//int b = 255;

//void on_trackbar(int, void*)
//{
  //  cv::Canny(image, cannyImage, a, b);
   // cv::imshow("image", cannyImage);
//}

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
        cv::Mat hsvImage;
        cv::cvtColor(img, hsvImage, cv::COLOR_BGR2HSV);

        cv::Mat imageRemovedStick;
        cv::inRange(hsvImage, config.tableColorMin, config.tableColorMax, imageRemovedStick);
        
        cv::Mat imageNoBackground;
        cv::bitwise_not(imageRemovedStick, imageNoBackground);

        cv::Mat hsvImageAdjusted = hsvImage.clone();
        for(int i = 0; i < hsvImageAdjusted.rows; ++i)
        {
            for(int k = 0; k < hsvImageAdjusted.cols; ++k)
            {
                if(imageNoBackground.at<uint8_t>(i, k) == 0)
                {
                    hsvImageAdjusted.at<cv::Vec3b>(i, k)[0] = 0;
                    hsvImageAdjusted.at<cv::Vec3b>(i, k)[1] = 0;
                    hsvImageAdjusted.at<cv::Vec3b>(i, k)[2] = 0;
                }
            }
        }

        cv::Mat toBgr;
        cv::Mat gray;
        cv::cvtColor(hsvImageAdjusted, toBgr, cv::COLOR_HSV2BGR);
        cv::cvtColor(toBgr, gray, cv::COLOR_BGR2GRAY);
        
        cv::Mat resizedImage;
        cv::resize(gray, resizedImage, cv::Size(), 2, 2);
        image = resizedImage.clone();

        cv::Mat canny;
        cv::Canny(resizedImage, canny, 0, 200);

        cv::Mat dilated;
        cv::Mat eroded;
        cv::dilate(canny, dilated, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
        cv::erode(dilated, eroded, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2)));

        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(eroded, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

        std::cout << contours.size() << std::endl;

        // there should be 2 contours, a cue grip and the cue end point
        if(contours.size() == 2)
        {
            double minDistance = canny.rows * canny.cols;
            int contourMinDistance = -1;

            for(std::size_t i = 0; i < contours.size(); ++i)
            {
                for(std::size_t k = 0; k < contours.at(i).size(); ++k)
                {

                    double distance = 0;
                    if((distance = distancePointAndLine(cv::Point(0, 0), cv::Point(canny.cols-1, 0), contours.at(i).at(k))) < minDistance ||
                    (distance = distancePointAndLine(cv::Point(canny.cols-1, 0), cv::Point(canny.cols-1, canny.rows-1), contours.at(i).at(k))) < minDistance ||
                    (distance = distancePointAndLine(cv::Point(canny.cols-1, canny.rows-1), cv::Point(0, canny.rows-1), contours.at(i).at(k))) < minDistance ||
                    (distance = distancePointAndLine(cv::Point(0, canny.rows-1), cv::Point(0, 0), contours.at(i).at(k))) < minDistance)
                    {
                        minDistance = distance;
                        contourMinDistance = (int) i;
                    }
                }
            }

            std::cout << contourMinDistance << std::endl;

            if(contourMinDistance != -1)
            {
                int contourToKeep = (contourMinDistance == 1 ? contourToKeep = 0 : contourToKeep = 1);

                cv::Mat maskContourToKeep = cv::Mat::zeros(canny.rows, canny.cols, canny.type());
                cv::drawContours(maskContourToKeep, contours, contourToKeep, 255, -1);

                cv::Mat eroded;
                cv::erode(maskContourToKeep, eroded, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2,2)));

                cv::Mat resizedMask;
                cv::resize(eroded, resizedMask, cv::Size(), 0.5, 0.5);

                for(std::size_t i = 0; i < resizedMask.rows; ++i)
                {
                    for(std::size_t k = 0; k < resizedMask.cols; ++k)
                    {
                        if(resizedMask.at<uint8_t>(i, k) == 0)
                        {
                            hsvImageAdjusted.at<cv::Vec3b>(i, k)[0] = 0;
                            hsvImageAdjusted.at<cv::Vec3b>(i, k)[1] = 0;
                            hsvImageAdjusted.at<cv::Vec3b>(i, k)[2] = 0;
                        }
                    }
                }
            } 
        }
        return hsvImageAdjusted;
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