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
        /**
         * @brief Ehance the image using a bileteral filter
         * 
         * @param img image to enhance
         * @param data unused
         * @return std::shared_ptr<void> nullptr
         */
        virtual std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
    private:
        /**
         * @brief Blur the image
         * 
         * @param img 
         */
        void blurImage(cv::Mat& img);
    };

    class CueSegmentation : public IImageProcessing
    {
    public:
        CueSegmentation();
        virtual ~CueSegmentation();
        /**
         * @brief segment the image into needed objects
         * 
         * @param img image to segment on
         * @param data unused
         * @return std::shared_ptr<void> nullptr
         */
        virtual std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
    private:
        /**
        * @brief Threshold the image
        * 
        * @param img Image to threshold
        */
        void thresholdImage(cv::Mat& img);
        /**
         * @brief Apply opening operator
         * 
         * @param image image to apply opening operator on
         */
        void openImage(cv::Mat& image);
        /**
         * @brief Find the contours of the cue
         * 
         * @param img image in which the contours are found
         * @return std::vector<std::vector<cv::Point>> 
         */
        std::vector<std::vector<cv::Point>> findCueContours(const cv::Mat& img);
    };

    class CueFeatureExtraction : public IImageProcessing
    {
    public:
        CueFeatureExtraction();
        virtual ~CueFeatureExtraction();
        /**
         * @brief extract the features of the cue
         * 
         * @param img image to extract features from
         * @param data contains the contours of the cue
         * @return std::shared_ptr<void> pointer to vector of corner points points
         */
        virtual std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
    private:
        /**
         * @brief Finds the corner points of the contours
         * 
         * @param contours the contours of the cue
         * @return std::vector<cv::Point> vector containing the corner points
         */
        std::vector<cv::Point> findCornerPoints(const std::vector<std::vector<cv::Point>>& contours);
    };

    class CueClassification : public IImageProcessing
    {
    public:
        CueClassification();
        virtual ~CueClassification();
        /**
         * @brief Classify image information based on the table corner
         * 
         * @param img unused
         * @param data pointer to a data structure containing the corner points and the original image 
         * @return std::shared_ptr<void> vector containing the endpoints of the cue. Sorted in a way that the first one is the front
         */
        virtual std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
    private:
        /**
         * @brief find the center of the cue
         * 
         * @param cornerPoints corners of the cue
         * @return cv::Point center of the cue
         */
        cv::Point classifyCue(const std::vector<cv::Point>& cornerPoints);
        /**
         * @brief find the endpoints of the cue
         * 
         * @param cornerPoints corners of the cue
         * @return std::vector<cv::Point> vector containing the endpoints of the cue
         */
        std::vector<cv::Point> determineEndPoints(const std::vector<cv::Point> & cornerPoints);
        /**
         * @brief determine a line between the endpoints of the cue
         * 
         * @param endPoints end points of the cue
         * @return Line line calculated based on the endpoints
         */
        Line calculateLine(const std::vector<cv::Point>& endPoints);
        /**
         * @brief Determines which of the points is the front
         * 
         * @param points the endpoints of the cue
         * @param image the original image
         * @param line the line calculated from the endpoints
         */
        void determineFront(std::vector<cv::Point>& points, cv::Mat& image, const Line& line);
    };

}

#endif