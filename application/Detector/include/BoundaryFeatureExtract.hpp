#ifndef BOUNDARYFEATUREEXTRACT_HPP
#define BOUNDARYFEATUREEXTRACT_HPP

#include <include/IImageProcessing.hpp>
#include <include/IDetector.hpp>

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

    struct Boundary : public Object
	{
		std::array<cv::Point, 4> corners;
		std::array<cv::Point, 6> pocketsLoc;
		double pocketRad;
	};

    class BoundaryFeatureExtract : public IImageProcessing
    {
    public:
        BoundaryFeatureExtract();
        virtual ~BoundaryFeatureExtract();
        virtual std::shared_ptr<void> process(cv::Mat& img);
    private:
        void calculateIntersects(const std::vector<Line>& lines, std::vector<cv::Point>& intersects);
        void calculateLine(std::vector<Line>& lines, const cv::Point& pointA, const cv::Point& pointB);
        std::vector<cv::Point> getBiggestContour(const std::vector<std::vector<cv::Point>>& contours);
        //data
    }; //BoundaryEnhancement
} // namespace Detector

#endif //BOUNDARYFEATUREEXTRACT_HPP
