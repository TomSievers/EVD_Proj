#include "boundarydetector.hpp"

namespace Detector
{

    BoundaryDetector::BoundaryDetector()
    {

    }

    BoundaryDetector::~BoundaryDetector()
    {

    }

    int BoundaryDetector::run(int argc, char *argv[])
    {
        std::clock_t start;
        double duration;
        src = cv::imread("D:/EVD_Proj/Photos_pool_table/setup9_1.jpg",cv::IMREAD_COLOR);
        cv::GaussianBlur(src, src, cv::Size(3,3), 10, 0);
        if( src.empty() )
        {
            std::cout << "Could not open or find the image!\n" << std::endl;
            std::cout << "Usage: " << argv[0] << " <Input image>" << std::endl;
            return -1;
        }

        while(true){
            start = std::clock();
            cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);
            cv::inRange(hsv, cv::Scalar(86, 119, 0), cv::Scalar(114, 255, 255), threshold);
            std::vector<std::vector<cv::Point>> contours;
            std::vector<cv::Vec4i> hierarchy;
            cv::findContours(threshold,contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
            std::vector<std::vector<cv::Point>> contours_poly(contours.size());
            drawing = cv::Mat::zeros(threshold.size(), CV_8UC3);
            cv::Mat contourDrawing = cv::Mat::zeros(threshold.size(), CV_8UC3);
            int8_t biggestContourId = -1;
            float biggestContourArea = 0;
            for(std::size_t i = 0; i < contours.size(); i++)
            {
                cv::drawContours(contourDrawing, contours, i, cv::Scalar(255,0,0),2, cv::LINE_8);
                double arcLength = cv::arcLength(contours[i], true);
                cv::approxPolyDP(cv::Mat(contours[i]), contours_poly[i], arcLength * 0.01, true);
                cv::drawContours(drawing, contours_poly, i, cv::Scalar(255,0,0), 2, cv::LINE_8);
                float ctArea = cv::contourArea(contours_poly[i]);
                if(ctArea > biggestContourArea)
                {
                    biggestContourArea = ctArea;
                    biggestContourId = i;
                }
            }

            if(biggestContourId != -1)
            {
                cv::Point pointA;
                cv::Point pointB;
                std::uint8_t iter = 1;

                std::vector<Line> lines;
                for(std::size_t i = 0; i < contours_poly[biggestContourId].size()-iter; i+=iter)
                {
                    if(i < contours_poly[biggestContourId].size()-iter)
                    {
                        pointA = contours_poly[biggestContourId].at(i);
                        pointB = contours_poly[biggestContourId].at(i+=iter);

                        if(sqrt(pow(pointB.x - pointA.x, 2) + pow(pointB.y - pointA.y, 2)) > 120)
                        {
                            calculateLine(&lines, pointA, pointB);

                            cv::line(drawing, pointA, pointB, cv::Scalar(0,255,0), 2);
                        }
                    }
                }
                calculateIntersects(lines);

            }
            duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;

            std::cout << duration << std::endl;
            cv::imshow("contours", drawing);
            cv::imshow("threshold", threshold);
            cv::imshow("contourdrawing", contourDrawing);
            cv::imshow("src", src);

            char key = (char)cv::waitKey(30);
            if(key == 'q')
            {
                break;
            }
        }
    }

    void BoundaryDetector::calculateLine(std::vector<Line>* lines, const cv::Point& pointA, const cv::Point& pointB)
    {
        double a = pointB.y - pointA.y;
        double b = pointA.x - pointB.x;
        double c = a * pointA.x + b * pointA.y;
        Line line(a,b,c);
        lines->push_back(line);
    }

    void BoundaryDetector::calculateIntersects(const std::vector<Line>& lines)
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
                    cv::circle(src, cv::Point((int)x,(int)y), 3, cv::Scalar(0,0,255),3);
                }
            }
            else
            {
                double determinant = lines[i].a * lines[0].b - lines[0].a * lines[i].b;

                if(determinant != 0)
                {
                    double x = (lines[0].b * lines[i].c - lines[i].b * lines[0].c)/determinant;
                    double y = (lines[i].a * lines[0].c - lines[0].a * lines[i].c)/determinant;
                    cv::circle(src, cv::Point((int)x,(int)y), 3, cv::Scalar(0,0,255),3);
                }
            }
        }
    }

} //Detector
