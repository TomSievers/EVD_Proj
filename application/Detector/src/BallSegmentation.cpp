#include "include/Ball/BallSegmentation.hpp"
#include "include/IDetector.hpp"
#include <cmath>

#include <iostream>
#include <opencv2/highgui.hpp>

// defines for the pool table color
#define BACKGROUND_MIN_H 105
#define BACKGROUND_MIN_S 0
#define BACKGROUND_MIN_V 108
#define BACKGROUND_MAX_H 115
#define BACKGROUND_MAX_S 225
#define BACKGROUND_MAX_V 151

// defines for the Hough circle transform
#define HOUGH_DP 2.4f
#define HOUGH_MINDIST 1
#define HOUGH_PARAM1 50
#define HOUGH_PARAM2 35
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

    std::shared_ptr<void> BallSegmentation::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        cv::Mat imageHSV;
        cv::cvtColor(img, imageHSV, cv::COLOR_BGR2HSV);

        // 1: remove background
        cv::Mat imageNoBackground = removeBackground(imageHSV);

        // 2: do a closing operation to remove black holes in balls
        cv::Mat closedBackground;
         
        cv::morphologyEx(imageNoBackground, closedBackground, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)));
        //
        
        //remove border blobs
        for(int x = 0; x < closedBackground.cols; ++x)
        {
            cv::floodFill(closedBackground, cv::Point(x, 0), cv::Scalar(0));
            cv::floodFill(closedBackground, cv::Point(x, closedBackground.rows-1), cv::Scalar(0));
        }

        for(int y = 0; y < closedBackground.rows; ++y)
        {
            cv::floodFill(closedBackground, cv::Point(0, y), cv::Scalar(0));
            cv::floodFill(closedBackground, cv::Point(closedBackground.cols-1, y), cv::Scalar(0));
        }

        cv::Mat dist;
        cv::Mat test;
        double min, max;
        cv::distanceTransform(closedBackground, dist, cv::DIST_FAIR, cv::DIST_MASK_PRECISE);
        // Normalize the distance image for range = {0.0, 1.0}
        // so we can visualize and threshold it
        cv::normalize(dist, dist, 0, 1.0, cv::NORM_MINMAX);
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));

        int borderSize = 20;
        cv::Mat distborder(dist.rows + 2*borderSize, dist.cols + 2*borderSize, dist.depth());
        cv::copyMakeBorder(dist, distborder, 
                borderSize, borderSize, borderSize, borderSize, 
                cv::BORDER_CONSTANT | cv::BORDER_ISOLATED, cv::Scalar(0, 0, 0));
        // create a template. from the sizes of the circles in the image, 
        // a ~75 radius disk looks reasonable, so the borderSize was selected as 75
        cv::Mat distTempl;
        cv::Mat kernel2 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2*borderSize+1, 2*borderSize+1));
        // erode the ~75 radius disk a bit
        cv::erode(kernel2, kernel2, kernel, cv::Point(-1, -1), 10);
        // take its distance transform. this is the template
        cv::distanceTransform(kernel2, distTempl, cv::DIST_L2, cv::DIST_MASK_PRECISE);
        // match template
        cv::Mat nxcor;
        cv::matchTemplate(distborder, distTempl, nxcor, cv::TM_CCOEFF_NORMED);
        cv::minMaxLoc(nxcor, &min, &max);

        cv::Mat peaks, peaks8u;
        cv::threshold(nxcor, peaks, max*.5, 255, cv::THRESH_BINARY);
        cv::convertScaleAbs(peaks, peaks8u);
        /*cv::cvtColor(img, dist, cv::COLOR_BGR2GRAY);
        cv::threshold(dist, test, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
        cv::threshold(dist, dist, 0, 80, cv::THRESH_BINARY);
        cv::bitwise_and(dist, test, dist);*/
        cv::imshow("Distance", dist);
        cv::morphologyEx(peaks8u, peaks8u, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)), cv::Point(-1, -1), 1);
        cv::imshow("Distance Transform Image", peaks8u);


        // 3: find the balls (circles)
        std::vector<cv::Vec3f> circles = findCircles(peaks8u);

        // 4: add the found balls to the data
        for(const cv::Vec3f circle : circles)
        {
            std::shared_ptr<BallObject> circlePtr = std::make_shared<BallObject>();
            circlePtr->point = cv::Point((int) std::round(circle[0]), (int) std::round(circle[1]));
            circlePtr->radius = circle[2];
            std::static_pointer_cast<std::vector<std::shared_ptr<BallObject>>>(data)->push_back(circlePtr);
        }

        return std::make_shared<cv::Mat>(closedBackground);
    }

    std::vector<cv::Vec3f> BallSegmentation::findCircles(const cv::Mat& image)
    {
        std::vector<cv::Vec3f> circles;
        std::vector<double> circlesArea;
        double averageArea = 0;
        std::vector<std::size_t> iters;

        // look for the circles
        cv::HoughCircles(image, circles, cv::HOUGH_GRADIENT, HOUGH_DP, HOUGH_MINDIST, HOUGH_PARAM1, 
                        HOUGH_PARAM2, HOUGH_MINRADIUS, HOUGH_MAXRADIUS);

        for(const cv::Vec3f& circle : circles)
        {
            double area = M_PI * std::pow(circle[2], 2);
            circlesArea.push_back(area);
            averageArea += area; 
        }
        
        averageArea /= (double) circles.size();
        
        for(std::vector<cv::Vec3f>::iterator firstIterator = circles.begin(); firstIterator != circles.end();)
        {
            bool updatedFirstIter = false;
            for(std::vector<cv::Vec3f>::iterator secondIterator = circles.begin(); secondIterator != circles.end();)
            {
                // check every time if firstIterator points to end, since it gets updated in this for loop
                if(firstIterator == circles.end())
                {
                    break;
                }

                std::size_t distance = std::distance(firstIterator, secondIterator);
                if(distance != 0)
                {
                    double distanceBetweenCenterPoints = std::sqrt(std::pow((*firstIterator)[0] - (*secondIterator)[0], 2) +
                                                                    std::pow((*firstIterator)[1] - (*secondIterator)[1], 2));

                    if(distanceBetweenCenterPoints < ((*firstIterator)[2] / 2)) // divide by 3 to avoid overlapping circle removal
                    {
                        updatedFirstIter = true;
                        std::size_t currentFirstIter = std::distance(circles.begin(), firstIterator);
                        std::size_t currentSecondIter = std::distance(circles.begin(), secondIterator);

                        // keep the circle which is the closest to the average circle area, and remove the other one
                        if(std::abs(averageArea - circlesArea.at(currentFirstIter)) < std::abs(averageArea - circlesArea.at(currentSecondIter)))
                        {
                            secondIterator = circles.erase(secondIterator);
                            circlesArea.erase(circlesArea.begin() + currentSecondIter);
                            if(distance > 0)
                                firstIterator = circles.begin() + currentFirstIter;
                            else
                                firstIterator = circles.begin() + currentFirstIter - 1;
                        } else {
                            firstIterator = circles.erase(firstIterator);
                            circlesArea.erase(circlesArea.begin() + currentFirstIter);
                            if(distance > 0)
                                secondIterator = circles.begin() + currentSecondIter - 1;
                            else
                                secondIterator = circles.begin() + currentSecondIter;
                        } 
                    } else {
                        secondIterator++;
                    }
                } else {
                    secondIterator++;
                }
            }
            
            if(!updatedFirstIter)
                firstIterator++;
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