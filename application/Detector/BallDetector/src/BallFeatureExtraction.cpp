#include "include/BallFeatureExtraction.hpp"

#define BALL_WHITE_H_MIN 10
#define BALL_WHITE_S_MIN 10
#define BALL_WHITE_V_MIN 10
#define BALL_WHITE_H_MAX 10
#define BALL_WHITE_S_MAX 10
#define BALL_WHITE_V_MAX 10

namespace Detector
{
    BallFeatureExtraction::BallFeatureExtraction()
    {

    }

    BallFeatureExtraction::~BallFeatureExtraction()
    {

    }

    cv::Mat BallFeatureExtraction::getImage(const cv::Mat& image)
    {
        // not implemented
    }

    void BallFeatureExtraction::updateImageData(const cv::Mat& image, std::vector<std::unique_ptr<Object>>& ballObjects)
    {
        determineWhitePercentage(image, ballObjects);
    }

    void BallFeatureExtraction::determineWhitePercentage(const cv::Mat& image, std::vector<std::unique_ptr<Object>>& ballObjects)
    {
        // TODO: GET RIGHT IMAGE

        // get all white parts of the image
        cv::Mat imageWhiteParts;
        cv::inRange(image, cv::Scalar(BALL_WHITE_H_MIN, BALL_WHITE_S_MIN, BALL_WHITE_V_MIN), 
                    cv::Scalar(BALL_WHITE_H_MAX, BALL_WHITE_S_MAX, BALL_WHITE_V_MAX), imageWhiteParts);

        
    }

    std::vector<Contours> BallFeatureExtraction::determineContours(const std::vector<std::vector<cv::Point>>& contours, std::vector<std::unique_ptr<Object>>& ballObjects)
    {
        std::vector<std::vector<cv::Point>> newContours;

        for(const std::vector<cv::Point>& contour : contours)
        {
            double 
        }
    }

    std::vector<cv::Point> findContourCenterPoints(const std::vector<cv::Point>& contours)
    {

    }
}