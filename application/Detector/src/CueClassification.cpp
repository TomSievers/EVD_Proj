#include <include/IDetector.hpp>
#include <include/Cue/CueProcessing.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
namespace Detector
{
    CueClassification::CueClassification()
    {

    }

    CueClassification::~CueClassification()
    {

    }

    std::shared_ptr<void> CueClassification::process(cv::Mat& image, std::shared_ptr<void> data)
    {
        if(data != nullptr)
        {            
            (void) image;
            cueClassificationData cueData = *std::static_pointer_cast<cueClassificationData>(data);
            std::shared_ptr<CueObject> cue = std::make_shared<CueObject>();
            std::vector<cv::Point> cornerPoints = *std::static_pointer_cast<std::vector<cv::Point>>(data);
            if(cornerPoints.size() == 4)
            {
                cue->center = classifyCue(cornerPoints);
                cue->endPoints = determineEndPoints(cornerPoints);
                if(cue->endPoints.size() == 2)
                {
                    cue->line = calculateLine(cue->endPoints);
                    determineFront(cue->endPoints, cueData.image, cue->line);
                }
                else return nullptr;

                std::cout << "found cue at " << cue->center.x << " " << cue->center.y << std::endl;
                return cue;
            }
        }
        return nullptr;
    }

    cv::Point CueClassification::classifyCue(const std::vector<cv::Point> &cornerPoints)
    {
        uint64_t totalX = 0, totalY = 0;
        for(uint8_t i = 0; i < cornerPoints.size(); i++)
        {
            totalX += cornerPoints[i].x;
            totalY += cornerPoints[i].y;
        }
        return cv::Point((int)(totalX/cornerPoints.size()), (int)(totalY/cornerPoints.size()));
    }

    std::vector<cv::Point> CueClassification::determineEndPoints(const std::vector<cv::Point> & cornerPoints)
    {
        std::vector<cv::Point> points;
        for(uint8_t i = 0; i < cornerPoints.size(); i++)
        {
            std::cout << i % (cornerPoints.size()) << " " << (i+1) % (cornerPoints.size()) << std::endl;
            if(std::sqrt((std::pow((cornerPoints[i % (cornerPoints.size())].x - cornerPoints[(i+1) % (cornerPoints.size())].x),2)+std::pow((cornerPoints[i % (cornerPoints.size())].y - cornerPoints[(i + 1) % (cornerPoints.size())].y),2))) < 30)
            {
                points.push_back(cv::Point((cornerPoints[i % (cornerPoints.size())].x + cornerPoints[(i+1) % (cornerPoints.size())].x)/2, (cornerPoints[i % (cornerPoints.size())].y + cornerPoints[(i+1) % (cornerPoints.size())].y)/2));
            }
            
        }
        return points;
    }

    Line CueClassification::calculateLine(const std::vector<cv::Point>& points)
    {
        double a = points[1].y - points[0].y;
        double b = points[0].x - points[1].x;
        double c = a * points[0].x + b * points[0].y;
        return Line(a,b,c);
    }

    void CueClassification::determineFront(std::vector<cv::Point>& points, cv::Mat& image, const Line& line)
    {
        int x,y;
        if(line.b < 0)
        {
            /*check whether the cue is pointed more horizontal or vertical. 
            *in case of more horizontal we want to change the x and vice versa
            */
            if(std::sqrt(std::pow(points[0].x - points[1].x,2)) > std::sqrt(std::pow(points[0].y - points[1].y,2)))
            {
                x = points[0].x-5;
                y = (int)(line.a * x / line.b - (line.c / line.b));
            }
            else
            {
                /*check whether the point is the below or above the other point.
                 * If it is above the other point there needs to be looked above the point and vice versa
                */
                if(points[0].y < points[1].y)
                {
                    y = points[0].y - 5;
                } else
                {
                    y = points[0].y + 5;
                }
                x = (int)(line.b * y / line.a - (line.c / line.a));
            }
        }else
        { 
            /*check whether the cue is pointed more horizontal or vertical. 
            *in case of more horizontal we want to change the x and vice versa
            */
            if(std::sqrt(std::pow(points[0].x - points[1].x,2)) > std::sqrt(std::pow(points[0].y - points[1].y,2)))
            {
                x = points[0].x-5;
                y = (int)(line.a * x / line.b + (line.c / line.b));
            }
            else
            {
                /*check whether the point is the below or above the other point.
                 * If it is above the other point there needs to be looked above the point and vice versa
                */
                if(points[0].y < points[1].y)
                {
                    y = points[0].y - 5;
                } else
                {
                    y = points[0].y + 5;
                }
                x = (int)(line.b * y / line.a + (line.c / line.a));
            }
        }
        //make sure the points coordinates are greater than 0
        if(x < 0) x*=-1;
        if(y < 0) y*=-1;
        
        //split the pixel of interest up into three different channels to threshold
        cv::Mat hsv;
        cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
        cv::Mat threshold;
        cv::inRange(hsv, cv::Scalar(160, 160, 150), cv::Scalar(180,255,255), threshold);
        if(x >= 0 && x <= threshold.cols && y >= 0 && y <= threshold.rows)
        {
            uint8_t color = threshold.at<uint8_t>(cv::Point(x,y));
            if(color == 0)
            {
                cv::Point temp = points[0];
                points[0] = points[1];
                points[1] = temp;
    #ifdef DEBUG
                std::cout << "SWAP POINTS" << std::endl;
    #endif
            }
        }
        
        
#ifdef DEBUG
        std::cout << x << " " << y << std::endl;
        cv::circle(image, points[0], 5, cv::Scalar(0,255,0), cv::FILLED, cv::LINE_8);
        cv::circle(image, points[1], 5, cv::Scalar(0,0,255), cv::FILLED, cv::LINE_8);
        cv::imshow("front", image);
#endif

    }
}
