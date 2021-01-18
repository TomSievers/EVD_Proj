#include <include/Boundary/BoundaryFeatureExtract.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

namespace Detector
{
    BoundaryFeatureExtract::BoundaryFeatureExtract()
    {

    }

    BoundaryFeatureExtract::~BoundaryFeatureExtract()
    {
        
    }

    std::vector<cv::Point> BoundaryFeatureExtract::getBiggestContour(const std::vector<std::vector<cv::Point>>& contours)
    {
        std::size_t biggestContourId = -1;
        double biggestContourArea = 0;
        std::vector<std::vector<cv::Point>> contours_poly(contours.size());
        for(std::size_t i = 0; i < contours.size(); ++i)
        {
            double arcLength = cv::arcLength(contours[i], true);
            cv::approxPolyDP(cv::Mat(contours[i]), contours_poly[i], arcLength * 0.01, true);
            double ctArea = cv::contourArea(contours_poly[i]);
            if(ctArea > biggestContourArea)
            {
                biggestContourArea = ctArea;
                biggestContourId = i;
            }
        }
        if(biggestContourId != static_cast<std::size_t>(-1))
        {
            return contours_poly[biggestContourId];
        }
        std::vector<cv::Point> empty;
        return empty;
        
    }

    void BoundaryFeatureExtract::calculateLine(std::vector<Line>& lines, const cv::Point& pointA, const cv::Point& pointB)
    {
        double a = pointB.y - pointA.y;
        double b = pointA.x - pointB.x;
        double c = a * pointA.x + b * pointA.y;
        Line line(a,b,c);
        lines.push_back(line);
    }

    void BoundaryFeatureExtract::calculateIntersects(const std::vector<Line>& lines, std::vector<cv::Point2f>& intersects)
    {
        for(std::size_t i = 0; i < lines.size(); i++)
        {
            if(i != lines.size()-1)
            {
                double determinant = lines[i].a * lines[i+1].b - lines[i+1].a * lines[i].b;
                if(determinant != 0)
                {
                    double x = (lines[i+1].b * lines[i].c - lines[i].b * lines[i+1].c)/determinant;
                    double y = (lines[i].a * lines[i+1].c - lines[i+1].a * lines[i].c)/determinant;
                    intersects.push_back(cv::Point2f(static_cast<float>(x),static_cast<float>(y)));
                }
            }
            else
            {
                double determinant = lines[i].a * lines[0].b - lines[0].a * lines[i].b;

                if(determinant != 0)
                {
                    double x = (lines[0].b * lines[i].c - lines[i].b * lines[0].c)/determinant;
                    double y = (lines[i].a * lines[0].c - lines[0].a * lines[i].c)/determinant;
                    intersects.push_back(cv::Point2f(static_cast<float>(x),static_cast<float>(y)));
                }
            }
        }
    }

    bool sortXPoint(cv::Point2f a, cv::Point2f b)
    {
        return a.x < b.x; 
    }

    bool sortYPoint(cv::Point2f a, cv::Point2f b)
    {
        return a.y < b.y; 
    }

    void BoundaryFeatureExtract::sortIntersects(std::vector<cv::Point2f>& intersects)
    {
        std::sort(intersects.begin(), intersects.end(), sortXPoint);

        std::vector<cv::Point2f> leftMost = {intersects[0], intersects[1]};
        std::vector<cv::Point2f> rightMost = {intersects[2], intersects[3]};

        std::sort(leftMost.begin(), leftMost.end(), sortYPoint);

        intersects[0] = leftMost[0];
        intersects[3] = leftMost[1];

        double dist = 0;
        std::size_t bottomRightIndex = -1;
        
        for(std::size_t i = 0; i < rightMost.size(); ++i)
        {
            double tempDist = sqrt(pow(rightMost[i].x - intersects[0].x, 2) + pow(rightMost[i].y - intersects[0].y, 2));
            if(dist < tempDist)
            {
                dist = tempDist;
                bottomRightIndex = i;
            }
        }
        intersects[2] = rightMost[bottomRightIndex];
        intersects[1] = rightMost[(bottomRightIndex + 1)%2];
    }

    std::shared_ptr<void> BoundaryFeatureExtract::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(img ,contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
        auto contour = getBiggestContour(contours);
#ifdef DEBUG
        std::vector<std::vector<cv::Point>> cont = {contour};
        cv::drawContours(img, cont, -1, cv::Scalar(160), 4);
#endif
        std::shared_ptr<std::vector<cv::Point2f>> intersects = std::make_shared<std::vector<cv::Point2f>>();
        if(!contour.empty())
        {
            std::vector<Line> lines;
            cv::Point pointA;
            cv::Point pointB;
            for(std::size_t i = 0; i < contour.size(); i++)
            {
                if(i == contour.size()-1)
                {
                    pointA = contour.at(i);
                    pointB = contour.at(0);
                } else {
                    pointA = contour.at(i);
                    pointB = contour.at(i + 1);
                }

                if(sqrt(pow(pointB.x - pointA.x, 2) + pow(pointB.y - pointA.y, 2)) > 110)
                {
                    calculateLine(lines, pointA, pointB);
                }
            }
            calculateIntersects(lines, *intersects);

            if(intersects->size() == 4)
            {
                sortIntersects(*intersects);

                return intersects;
            }
        }
        return nullptr;
    }
} // namespace Detector
