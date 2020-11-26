#include <include/BoundaryFeatureExtract.hpp>
#include <opencv2/imgproc.hpp>
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
        int64_t biggestContourId = -1;
        double biggestContourArea = 0;
        std::vector<std::vector<cv::Point>> contours_poly(contours.size());
        for(std::size_t i = 0; i < contours.size(); ++i)
        {
            double arcLength = cv::arcLength(contours[i], true);
            cv::approxPolyDP(cv::Mat(contours[i]), contours_poly[i], arcLength * 0.008, true);
            double ctArea = cv::contourArea(contours_poly[i]);
            std::cout << "ct " << ctArea << std::endl;
            if(ctArea > biggestContourArea)
            {
                biggestContourArea = ctArea;
                biggestContourId = i;
            }
        }
        if(biggestContourId != -1)
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

    void BoundaryFeatureExtract::calculateIntersects(const std::vector<Line>& lines, std::vector<cv::Point>& intersects)
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
                    intersects.push_back(cv::Point(static_cast<int>(x),static_cast<int>(y)));
                }
            }
            else
            {
                double determinant = lines[i].a * lines[0].b - lines[0].a * lines[i].b;

                if(determinant != 0)
                {
                    double x = (lines[0].b * lines[i].c - lines[i].b * lines[0].c)/determinant;
                    double y = (lines[i].a * lines[0].c - lines[0].a * lines[i].c)/determinant;
                    intersects.push_back(cv::Point(static_cast<int>(x),static_cast<int>(y)));
                }
            }
        }
    }

    std::shared_ptr<void> BoundaryFeatureExtract::process(cv::Mat& img)
    {
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(img ,contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
        auto contour = getBiggestContour(contours);
        std::vector<std::vector<cv::Point>> cont = {contour};
        cv::drawContours(img, cont, -1, cv::Scalar(160), 4);
        std::vector<cv::Point> intersects;
        if(!contour.empty())
        {
            std::cout << "not empty" << std::endl;
            uint8_t iter = 1;
            std::vector<Line> lines;
            for(std::size_t i = 0; i < contour.size()-iter; i+=iter)
            {
                if(i < contour.size()-iter)
                {
                    cv::Point pointA = contour.at(i);
                    cv::Point pointB = contour.at(i+=iter);

                    std::cout << "points" << std::endl;

                    if(sqrt(pow(pointB.x - pointA.x, 2) + pow(pointB.y - pointA.y, 2)) > 120)
                    {
                        std::cout << "line" << std::endl;
                        calculateLine(lines, pointA, pointB);
                    }
                }
            }
            calculateIntersects(lines, intersects);

            for(auto intersect : intersects)
            {
                cv::circle(img, intersect, 4, cv::Scalar(255, 0, 0), 2);
                std::cout << intersect.x << " " << intersect.y << std::endl;
            }

            Boundary boundary;
            //boundary.corners = intersects.data();

        }
        return nullptr;
    }
} // namespace Detector
