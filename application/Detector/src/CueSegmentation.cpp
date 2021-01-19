#include <include/IDetector.hpp>
#include <include/Cue/CueProcessing.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#define DEBUG
#define CUE_TIP_H_MIN 0 // old: 15
#define CUE_TIP_S_MIN 0 // old 23
#define CUE_TIP_V_MIN 50 // old 90
#define CUE_TIP_H_MAX 180 // old 37
#define CUE_TIP_S_MAX 150 // old 155
#define CUE_TIP_V_MAX 255 // old 255

namespace Detector
{
    CueSegmentation::CueSegmentation()
    {

    }

    CueSegmentation::~CueSegmentation()
    {

    }

    std::shared_ptr<void> CueSegmentation::process(cv::Mat& image, std::shared_ptr<void> data)
    {
        (void)data;
        thresholdImage(image);
        openImage(image);
        std::vector<std::vector<cv::Point>> contours = findCueContours(image);
        if(!contours.empty())
        {
            return std::make_shared<std::vector<std::vector<cv::Point>>>(contours);
        }
        return nullptr;
    }

    void CueSegmentation::thresholdImage(cv::Mat& image)
    {
        cv::Mat hsvImage;
        cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);
        cv::inRange(hsvImage, cv::Scalar(CUE_TIP_H_MIN, CUE_TIP_S_MIN, CUE_TIP_V_MIN), cv::Scalar(CUE_TIP_H_MAX, CUE_TIP_S_MAX, CUE_TIP_V_MAX), image);

    }

    void CueSegmentation::openImage(cv::Mat& image)
    {
        cv::dilate(image, image, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7,7)));
        cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(11,11));
        cv::morphologyEx(image, image, cv::MORPH_OPEN, element);
        cv::erode(image, image, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7,7)));
    }

    std::vector<std::vector<cv::Point>> CueSegmentation::findCueContours(const cv::Mat& image)
    {
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(image,contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
        return contours;
    }
}
