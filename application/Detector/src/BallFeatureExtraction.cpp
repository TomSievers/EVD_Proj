#include "include/Ball/BallFeatureExtraction.hpp"

#include <opencv2/highgui.hpp>
#include <iostream>

#define BALL_WHITE_H_MIN 9 // old: 15
#define BALL_WHITE_S_MIN 13 // old 23
#define BALL_WHITE_V_MIN 103 // old 90
#define BALL_WHITE_H_MAX 118 // old 37
#define BALL_WHITE_S_MAX 85 // old 155
#define BALL_WHITE_V_MAX 255 // old 255

namespace Detector
{
    BallFeatureExtraction::BallFeatureExtraction()
    {

    }

    BallFeatureExtraction::~BallFeatureExtraction()
    {

    }

    std::shared_ptr<void> BallFeatureExtraction::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        if(enhancementImg)
        {
            std::shared_ptr<std::vector<std::shared_ptr<BallObject>>> ballObjectPtr = std::static_pointer_cast<std::vector<std::shared_ptr<BallObject>>>(data);
            determineWhitePercentage(img, *ballObjectPtr);
        }
        return nullptr;
    }

    void BallFeatureExtraction::setEnhancementImage(const cv::Mat& img)
    {
        enhancementImg = std::make_shared<cv::Mat>(img);
    }

    void BallFeatureExtraction::determineWhitePercentage(const cv::Mat& image, std::vector<std::shared_ptr<BallObject>>& ballObjects)
    {
        if(enhancementImg)
        {
            // get the contours of the balls
            /*std::vector<std::vector<cv::Point>> ballContours;
            cv::findContours(image, ballContours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);*/

            // convert enhanced image to hsv
            cv::Mat enhancedImageHsv;
            cv::cvtColor(*enhancementImg, enhancedImageHsv, cv::COLOR_BGR2HSV);

            // get all white parts of the image and do a closing operation
            cv::Mat imageWhiteParts;
            cv::inRange(enhancedImageHsv, cv::Scalar(BALL_WHITE_H_MIN, BALL_WHITE_S_MIN, BALL_WHITE_V_MIN), 
                        cv::Scalar(BALL_WHITE_H_MAX, BALL_WHITE_S_MAX, BALL_WHITE_V_MAX), imageWhiteParts);
            
            cv::Mat imageWhitePartsClosed;
            cv::morphologyEx(imageWhiteParts, imageWhitePartsClosed, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)));

            for(auto ball : ballObjects)
            {
                uint64_t whiteCount = 0;
                uint64_t blackCount = 0;
                uint64_t count = 0;
                for(int x = (int)round((double)ball->point.x - ball->radius); x < (int)round((double)ball->point.x + ball->radius); ++x)
                {
                    for(int y = (int)round((double)ball->point.y - ball->radius); y < (int)round((double)ball->point.y + ball->radius); ++y)
                    {
                        if(sqrt(pow((double)(x - ball->point.x), 2) +  pow((double)(y - ball->point.y), 2)) <= ball->radius)
                        {
                            ++count;
                            if(imageWhitePartsClosed.at<uint8_t>(y, x) == 255)
                            {
                                ++whiteCount;
                            } else {
                                ++blackCount;
                            }   
                        }
                    }
                }

                ball->percentageWhite = (uint8_t)round(((float)whiteCount/(float)count)*100.0F);
            }
            
            /*std::vector<std::vector<cv::Point>> whiteContours;
            cv::findContours(imageWhitePartsClosed, whiteContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

            // check for every ball which ball contour points and white parts are in his region

            for(std::size_t i = 0; i < ballObjects.size(); ++i)
            {
                double radiusSquared = std::pow(ballObjects.at(i)->radius, 2);
                // check which ball contour points are in region of ball
                for(std::size_t k = 0; k < ballContours.size(); ++k)
                {
                    for(std::size_t y = 0; y < ballContours.at(k).size(); ++y)
                    {
                        double distanceBetweenPoints = std::pow(ballContours.at(k).at(y).x - ballObjects.at(i)->point.x, 2) +
                                                        std::pow(ballContours.at(k).at(y).y - ballObjects.at(i)->point.y, 2);
                        
                        if(distanceBetweenPoints <= (radiusSquared + radiusSquared / 2)) // since the found circles are not always perfect, add some value to the max distance
                        {
                            ballObjects.at(i)->ballContourPoints.push_back(ballContours.at(k).at(y));
                        }
                    }
                }

                // check which white contour parts parts are in region of ball
                for(std::size_t k = 0; k < whiteContours.size(); ++k)
                {
                    for(std::size_t y = 0; y < whiteContours.at(k).size(); ++y)
                    {
                        double distanceBetweenPoints = std::pow(whiteContours.at(k).at(y).x - ballObjects.at(i)->point.x, 2) +
                                                        std::pow(whiteContours.at(k).at(y).y - ballObjects.at(i)->point.y, 2);
                        
                        if(distanceBetweenPoints <= (radiusSquared)) // since the found circles are not always perfect, add some value to the max distance
                        {
                            ballObjects.at(i)->whiteContourPoints.push_back(whiteContours.at(k).at(y));
                        }
                    }
                }

                // determine the white percentage of the ball
                if(ballObjects.at(i)->ballContourPoints.size() != 0)
                {
                    double ballContourArea = cv::contourArea(ballObjects.at(i)->ballContourPoints);
                    double whiteContourArea = 0;
                    
                    if(ballObjects.at(i)->whiteContourPoints.size() != 0)
                    {
                        whiteContourArea = cv::contourArea(ballObjects.at(i)->whiteContourPoints);
                        std::cout << whiteContourArea << " " << ballContourArea << " " << i << std::endl;
                    }

                    ballObjects.at(i)->percentageWhite = (uint8_t) std::round(whiteContourArea / ballContourArea * 100);
                } else {
                    ballObjects.at(i)->percentageWhite = 0;
                }
            }*/

#ifdef DEBUG
            cv::Mat bitwised_xor_image;
            cv::bitwise_xor(image, imageWhitePartsClosed, bitwised_xor_image);
            for(std::size_t i = 0; i < ballObjects.size(); ++i)
            {

                cv::putText(*enhancementImg, //target image
                            std::to_string(i) + " " + std::to_string(ballObjects.at(i)->percentageWhite), //text
                            ballObjects.at(i)->point, //top-left position
                            cv::FONT_HERSHEY_DUPLEX,
                            1.0,
                            CV_RGB(118, 185, 0), //font color
                            2);

            }
            cv::imshow("enhanced image", *enhancementImg);
            cv::imshow("image white parts", imageWhitePartsClosed);
#endif
        }
    }
}