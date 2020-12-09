#include <include/IDetector.hpp>
#include <include/Cue/CueProcessing.hpp>
#include "opencv2/imgproc.hpp"

namespace Detector
{
    CueFeatureExtraction::CueFeatureExtraction()
    {

    }

    CueFeatureExtraction::~CueFeatureExtraction()
    {

    }
    std::shared_ptr<void> CueFeatureExtraction::process(cv::Mat& image, std::shared_ptr<void> data)
    {
        if(data != nullptr)
        {
            (void)image;
            std::vector<std::vector<cv::Point>> contours = *std::static_pointer_cast<std::vector<std::vector<cv::Point>>>(data);
            std::vector<cv::Point> cornerPoints = findCornerPoints(contours);
            if(cornerPoints.size() == 4)
            {
                return std::make_shared<std::vector<cv::Point>>(cornerPoints);
            }
            return nullptr;
        }
        return nullptr;
    }
    std::vector<cv::Point> CueFeatureExtraction::findCornerPoints(const std::vector<std::vector<cv::Point> > &contours)
    {
        std::vector<cv::Point> cornerPoints;
        std::vector<std::vector<cv::Point>> approxPolyContours;
        approxPolyContours.resize(contours.size());
        for(std::size_t i = 0; i < contours.size(); i++)
        {
            double epsilon = cv::arcLength(contours[i], true);
            cv::approxPolyDP(contours[i], approxPolyContours[i], epsilon*0.03, true);
        }
        uint8_t biggestContourId = 0;
        for(uint8_t i = 0; i < approxPolyContours.size(); i++)
        {
            if(approxPolyContours[i].size() == 4)
            {
                if(cv::contourArea(approxPolyContours[i]) > cv::contourArea(approxPolyContours[biggestContourId]))
                {
                    biggestContourId = i;
                }
            }
        }

        for(std::size_t i = 0; i < approxPolyContours[biggestContourId].size(); i++)
        {
            cornerPoints.push_back(cv::Point(approxPolyContours[biggestContourId][i].x, approxPolyContours[biggestContourId][i].y));
        }
        return cornerPoints;
    }
}
