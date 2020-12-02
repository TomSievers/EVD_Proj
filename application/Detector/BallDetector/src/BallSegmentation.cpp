#include "include/BallSegmentation.hpp"
#include "include/BallObject.hpp"
#include <cmath>

// defines for the pool table color
#define BACKGROUND_MIN_H 10
#define BACKGROUND_MIN_S 10
#define BACKGROUND_MIN_V 10
#define BACKGROUND_MAX_H 10
#define BACKGROUND_MAX_S 10
#define BACKGROUND_MAX_V 10

// defines for the Hough circle transform
#define HOUGH_DP 3.2
#define HOUGH_MINDIST 1
#define HOUGH_PARAM1 50
#define HOUGH_PARAM2 39
#define HOUGH_MINRADIUS 0
#define HOUGH_MAXRADIUS 20

namespace Detector
{
    BallSegmentation::BallSegmentation()
    {

    }

    BallSegmentation::~BallSegmentation()
    {

    }

    cv::Mat BallSegmentation::getImage(const cv::Mat& image)
    {
        cv::Mat imageHSV;
        cv::cvtColor(image, imageHSV, cv::COLOR_BGR2HSV);
        return removeBackground(imageHSV);
    }

    void BallSegmentation::updateImageData(const cv::Mat& image, std::vector<std::unique_ptr<Object>>& ballObjects)
    {
        std::vector<cv::Vec3f> circles = findCircles(image);
        
        for(const cv::Vec3f circle : circles)
        {
            std::unique_ptr<Object> circlePtr = std::make_unique<BallObject>();
            dynamic_cast<BallObject*>(circlePtr.get())->point = cv::Point((int) std::round(circle[0]), (int) std::round(circle[1]));
            dynamic_cast<BallObject*>(circlePtr.get())->radius = circle[2];
            ballObjects.push_back(std::move(circlePtr));
        }
    }

    std::vector<cv::Vec3f> BallSegmentation::findCircles(const cv::Mat& image)
    {
        std::vector<cv::Vec3f> circles;
        std::vector<std::size_t> iters;
        cv::HoughCircles(image, circles, cv::HOUGH_GRADIENT, HOUGH_DP, HOUGH_MINDIST, HOUGH_PARAM1, 
                        HOUGH_PARAM2, HOUGH_MINRADIUS, HOUGH_MAXRADIUS);

        for(std::size_t i = 0; i < circles.size(); ++i)
        {
            for(std::size_t k = 0; k < circles.size(); ++k)
            {
                double distanceBetweenCenterPoints = std::sqrt(std::pow(circles.at(i)[0] - circles.at(k)[0], 2) +
                                                                 std::pow(circles.at(i)[1] - circles.at(k)[1], 2));
                
                bool containsIter = false;
                for(std::size_t iter : iters)
                {
                    if(iter == i)
                        containsIter = true;
                }
                
                if(distanceBetweenCenterPoints < circles.at(i)[2] && !containsIter)
                {
                    iters.push_back(i);
                }
            }
        }

        // maybe optimize this to use a list instead of a vector because a vector needs to be copied
        for(std::size_t iter : iters)
        {
            circles.erase(circles.begin() + iter);
        }

        return circles;
    }

    cv::Mat BallSegmentation::removeBackground(const cv::Mat& image)
    {
        cv::Mat imageBackground;
        cv::inRange(image, cv::Scalar(BACKGROUND_MIN_H, BACKGROUND_MIN_S, BACKGROUND_MIN_V), 
                    cv::Scalar(BACKGROUND_MAX_H, BACKGROUND_MAX_S, BACKGROUND_MAX_V), imageBackground);
        
        cv::Mat backgroundReversed;
        cv::bitwise_not(imageBackground, backgroundReversed);
        return backgroundReversed;
    }
}