#include <include/IDetector.hpp>
#include <include/Cue/CueProcessing.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
#define DEBUG
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
            cue->center = classifyCue(cornerPoints);
            cue->endPoints = determineEndPoints(cornerPoints);
            cue->line = calculateLine(cue->endPoints);
            determineFront(cue->endPoints, cueData.image, cue->line);
            return cue;
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
        std::cout << "size : "<< cornerPoints.size() << std::endl;
        std::vector<cv::Point> points;
        for(uint8_t i = 0; i < cornerPoints.size(); i++)
        {
            if(i == 3)
            {
                if(std::sqrt((std::pow((cornerPoints[i].x - cornerPoints[0].x),2)+std::pow((cornerPoints[i].y - cornerPoints[0].y),2))) < 13)
                {
                    points.push_back(cv::Point((cornerPoints[i].x + cornerPoints[0].x)/2, (cornerPoints[i].y + cornerPoints[0].y)/2));
                    std::cout << (cornerPoints[i].x + cornerPoints[0].x)/2 << "," << (cornerPoints[i].y + cornerPoints[0].y)/2 << std::endl;
                }
            }else
            {
                if(std::sqrt(std::pow((cornerPoints[i].x - cornerPoints[i+1].x),2)+std::pow((cornerPoints[i].y - cornerPoints[i+1].y),2)) < 13)
                {
                    points.push_back(cv::Point((cornerPoints[i].x + cornerPoints[i+1].x)/2, (cornerPoints[i].y + cornerPoints[i+1].y)/2));
                    std::cout << (cornerPoints[i].x + cornerPoints[i+1].x)/2 << "," << (cornerPoints[i].y + cornerPoints[i+1].y)/2 << std::endl;
                }
            }
        }
        return points;
    }

/*
CHECK TOEVOEGEN DAT SIZE POINTS 2 IS ANDERS WERKT HIJ NIET
*/

    Line CueClassification::calculateLine(const std::vector<cv::Point>& points)
    {
        double a = points[1].y - points[0].y;
        double b = points[0].x - points[1].x;
        double c = a * points[0].x + b * points[0].y;
        return Line(a,b,c);
    }
/*
CHECK TOEVOEGEN OF LINE WEL BESTAAT
*/
    void CueClassification::determineFront(std::vector<cv::Point>& points, cv::Mat& image, const Line& line)
    {
        int x,y;
        std::cout << "size: " << points.size() << std::endl;
        if(line.b < 0)
        {
            /*check whether the cue is pointed more horizontal or vertical. 
            *in case of more horizontal we want to change the x and vice versa
            */
            if(std::sqrt(std::pow(points[0].x - points[1].x,2)) > std::sqrt(std::pow(points[0].y - points[1].y,2)))
            {
                x = points[0].x-5;
                y = line.a * x / line.b - (line.c / line.b);
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
                x = line.b * y / line.a - (line.c / line.a);
            }
        }else
        { 
            /*check whether the cue is pointed more horizontal or vertical. 
            *in case of more horizontal we want to change the x and vice versa
            */
            if(std::sqrt(std::pow(points[0].x - points[1].x,2)) > std::sqrt(std::pow(points[0].y - points[1].y,2)))
            {
                x = points[0].x-5;
                y = line.a * x / line.b + (line.c / line.b);
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
                x = line.b * y / line.a + (line.c / line.a);
            }
        }
        //make sure the points coordinates are greater than 0
        if(x < 0) x*=-1;
        if(y < 0) y*=-1;
        
        //split the pixel of interest up into three different channels to threshold
        cv::Vec3b color = image.at<cv::Vec3b>(cv::Point(x,y));
        int blue = color[0], green = color[1], red = color[2];

        #ifdef DEBUG
        std::cout << "Red:" << red << std::endl;
        std::cout << "Green:" << green << std::endl;
        std::cout << "Blue:" << blue << std::endl;
        #endif

        //check if the all values are within a certain threshold
        //if not, the point is the backside of the cue. So the points in the array have to be swapped
        if(blue >= 80 && blue <= 165 && 
        green >=40 && green <= 200 && 
        red >= 0 && red <= 80)
        {
        } 
        else
        {
            #ifdef DEBUG
            std::cout << "SWAP POINTS" << std::endl;
            #endif
            cv::Point temp = points[0];
            points[0] = points[1];
            points[1] = temp;
        }
        
        #ifdef DEBUG
        cv::circle(image, cv::Point(x, y), 3, cv::Scalar(255,0,0), cv::FILLED, cv::LINE_8);
        cv::imshow("checked pixel", image);
        #endif

    }
}
