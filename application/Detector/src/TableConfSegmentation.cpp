#include "include/Configure/TableConfSegmentation.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <cstdint>
#include <iostream>

namespace Detector
{
    TableConfSegmentation::TableConfSegmentation()
    {

    }

    TableConfSegmentation::~TableConfSegmentation()
    {

    }

    std::shared_ptr<void> TableConfSegmentation::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        cv::Mat imgHsv;
        cv::cvtColor(img, imgHsv, cv::COLOR_BGR2HSV);

        cv::Mat shiftedImage = shiftImage(imgHsv);

        std::shared_ptr<Config> configPtr = std::static_pointer_cast<Config>(data);
        cv::Mat ROI = findROI(shiftedImage, imgHsv, *configPtr);
        ROI.copyTo(img);
        return nullptr;
    }

    cv::Mat TableConfSegmentation::shiftImage(const cv::Mat& img)
    {
        cv::Mat splitted[3];
        cv::split(img, splitted);

        uint8_t shift = 25;

        for(int i = 0; i < splitted[0].rows; ++i)
        {
            for(int k = 0; k < splitted[0].cols; ++k)
            {
                splitted[0].at<uint8_t>(i, k) = ((uint16_t) splitted[0].at<uint8_t>(i, k) + shift) % 180;
            }
        }

        return splitted[0];
    }

    cv::Mat TableConfSegmentation::findROI(const cv::Mat& shiftedImage, const cv::Mat& enhancedImage, Config& config)
    {
        cv::Mat thresholdedImg;
        cv::threshold(shiftedImage, thresholdedImg, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

        // multiple time close to remove unfilled parts
        cv::Mat closedImg;
        cv::morphologyEx(thresholdedImg, closedImg, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9)), cv::Point(-1, -1), 9);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(closedImg, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        if(contours.size() == 0)
        {
            return cv::Mat();
        }

        std::size_t maxContourPos = 0;
        uint32_t maxContourArea= 0;

        // find the contour with the largest area
        for(std::size_t i = 0; i < contours.size(); ++i)
        {
            if(cv::contourArea(contours.at(i)) > maxContourArea)
            {
                maxContourPos = i;
                maxContourArea = (uint32_t) cv::contourArea(contours.at(i));
            }
        }
   
        cv::Moments moments = cv::moments(contours.at(maxContourPos));
        int cX = (int) (moments.m10 / moments.m00);
        int cY = (int) (moments.m01 / moments.m00);

        int minX = shiftedImage.cols;
        int maxX = 0;
        int minY = shiftedImage.rows;
        int maxY = 0;

        for(std::size_t i = 0; i < contours.at(maxContourPos).size(); ++i)
        {
            if((int) contours.at(maxContourPos).at(i).x > maxX)
            {
                maxX = (int) contours.at(maxContourPos).at(i).x;
            }
            else if((int) contours.at(maxContourPos).at(i).x < minX)
            {
                minX = (int) contours.at(maxContourPos).at(i).x;
            }
            else if((int) contours.at(maxContourPos).at(i).y > maxY)
            {
                maxY = (int) contours.at(maxContourPos).at(i).y;
            }
            else if((int) contours.at(maxContourPos).at(i).y < minY)
            {
                minY = (int) contours.at(maxContourPos).at(i).y;
            }
        }

        int roiXmin = (int) ((minX + cX) / 2.0);
        int roiXmax = (int) ((maxX + cX) / 2.0);
        int roiYmin = (int) ((minY + cY) / 2.0);
        int roiYmax = (int) ((maxY + cY) / 2.0);

        config.roiPositions[0] = roiXmin;
        config.roiPositions[1] = roiXmax;
        config.roiPositions[2] = roiYmin;
        config.roiPositions[3] = roiYmax;

        // return the region of interest
        return enhancedImage(cv::Rect(roiXmin, roiYmin, roiXmax - roiXmin, roiYmax - roiYmin)).clone();
    }
}