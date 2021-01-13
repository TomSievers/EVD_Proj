#include "include/Ball/BallFeatureExtraction.hpp"

#include <opencv2/highgui.hpp>

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
            std::vector<std::vector<cv::Point>> ballContours;
            cv::findContours(image, ballContours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

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
                            if(y < imageWhitePartsClosed.rows && x < imageWhitePartsClosed.cols && y >= 0 && x >= 0)
                            {
                                //imageWhitePartsClosed.at<uint8_t>(y, x) = 125;
                                ++count;
                                if(imageWhitePartsClosed.at<uint8_t>(y, x) == 255)
                                {
                                    ++whiteCount;
                                }
                            }
                            
                        }
                    }
                }

                ball->percentageWhite = (uint8_t)round(((float)whiteCount/(float)count)*100.0F);
            }

#ifdef DEBUG
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

            cv::Mat bitwised_xor_image;
            cv::bitwise_xor(image, imageWhitePartsClosed, bitwised_xor_image);

            cv::imshow("enhanced image", *enhancementImg);
            cv::imshow("image white parts", bitwised_xor_image);
#endif
        }
    }
}