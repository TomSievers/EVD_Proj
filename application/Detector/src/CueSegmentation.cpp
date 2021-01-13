#include <include/IDetector.hpp>
#include <include/Cue/CueProcessing.hpp>
#include "include/Configure/Configuration.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

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
        cv::imshow("thresholded cue", image);
        cv::waitKey(0);
        openImage(image);
        cv::imshow("opened cue", image);
        cv::waitKey(0);
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
        //cv::inRange(hsvImage, cv::Scalar(72, 58, 78), cv::Scalar(105, 255,255), image);
        cv::inRange(hsvImage, Configuration::getInstance().getConfig()->cueColorMin, Configuration::getInstance().getConfig()->cueColorMax, image);
    }

    void CueSegmentation::openImage(cv::Mat& image)
    {
        //cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(11,11));
        //cv::morphologyEx(image, image, cv::MORPH_OPEN, element);

        cv::Mat dilatedImage;
        cv::dilate(image, dilatedImage, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)), cv::Point(-1, -1), 2);

        cv::Mat erodedImage;
        cv::erode(dilatedImage, erodedImage, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)), cv::Point(-1, -1), 1);

        erodedImage.copyTo(image);
    }

    std::vector<std::vector<cv::Point>> CueSegmentation::findCueContours(const cv::Mat& image)
    {
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(image,contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
        return contours;
    }
}
