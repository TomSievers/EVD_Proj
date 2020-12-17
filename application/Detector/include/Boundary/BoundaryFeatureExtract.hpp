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

    class BoundaryFeatureExtract : public IImageProcessing
    {
    public:
        BoundaryFeatureExtract();
        virtual ~BoundaryFeatureExtract();
        /**
         * @brief extract the features of the table
         * 
         * @param img image to extract features from
         * @param data unused
         * @return std::shared_ptr<void> pointer to vector of float points
         */
        virtual std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
    private:
        void calculateIntersects(const std::vector<Line>& lines, std::vector<cv::Point2f>& intersects);
        void calculateLine(std::vector<Line>& lines, const cv::Point& pointA, const cv::Point& pointB);
        void sortIntersects(std::vector<cv::Point2f>& intersects);
        std::vector<cv::Point> getBiggestContour(const std::vector<std::vector<cv::Point>>& contours);
        //data
    }; //BoundaryEnhancement
} // namespace Detector

#endif //BOUNDARYFEATUREEXTRACT_HPP
