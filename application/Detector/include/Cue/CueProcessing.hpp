#ifndef CUEPROCESSING_HPP
#define CUEPROCESSING_HPP

#include <include/IImageProcessing.hpp>
#include <iostream>
namespace Detector
{

    struct cueClassificationData
    {
        std::vector<cv::Point> cornerPoints;
        cv::Mat& image;
        cueClassificationData(std::vector<cv::Point> corners, cv::Mat& img) :
        cornerPoints(corners), image(img)
        {
        }
        ~cueClassificationData()
        {

        }
    };

    class CueEnhancement : public IImageProcessing
    {
    public:
        CueEnhancement();
        virtual ~CueEnhancement();
        virtual std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
    private:
        void blurImage(cv::Mat& img);
    };

    class CueSegmentation : public IImageProcessing
    {
    public:
        CueSegmentation();
        virtual ~CueSegmentation();
        virtual std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
    private:
        void thresholdImage(cv::Mat& img);
        void openImage(cv::Mat& image);
        std::vector<std::vector<cv::Point>> findCueContours(const cv::Mat& img);
    };

    class CueFeatureExtraction : public IImageProcessing
    {
    public:
        CueFeatureExtraction();
        virtual ~CueFeatureExtraction();
        virtual std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
    private:
        std::vector<cv::Point> findCornerPoints(const std::vector<std::vector<cv::Point>>& contours);
    };

    class CueClassification : public IImageProcessing
    {
    public:
        CueClassification();
        virtual ~CueClassification();
        virtual std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
    private:
        cv::Point classifyCue(const std::vector<cv::Point>& cornerPoints);
        std::vector<cv::Point> determineEndPoints(const std::vector<cv::Point> & cornerPoints);
        Line calculateLine(const std::vector<cv::Point>& cornerPoints);
        void determineFront(std::vector<cv::Point>&, cv::Mat& image, const Line& line);
    };

}

#endif