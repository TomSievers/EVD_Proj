#ifndef BOUNDARYDETECTOR_HPP
#define BOUNDARYDETECTOR_HPP

#include <vector>
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"

#include <math.h>
#include <cstdio>
#include <ctime>
#include <iostream>

namespace Detector
{

    struct Line
    {
        double a, b, c;

        Line(double a, double b, double c) :
               a(a), b(b), c(c)
        {};
        ~Line(){};
    };

    class BoundaryDetector
    {
    public:
        BoundaryDetector();
        ~BoundaryDetector();
        int run(int argc, char *argv[]);

    private:
        void calculateLine(std::vector<Line>* lines, const cv::Point& pointA, const cv::Point& pointB);
        void calculateIntersects(const std::vector<Line>& lines);

        cv::Mat src;
        cv::Mat drawing;
        cv::Mat hsv;
        cv::Mat threshold;
    };

}//Detector
#endif // BOUNDARYDETECTOR_HPP
