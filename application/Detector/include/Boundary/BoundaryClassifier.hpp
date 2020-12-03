#ifndef BOUNDARYCLASSIFIER_HPP
#define BOUNDARYCLASSIFIER_HPP

#include <include/IImageProcessing.hpp>

namespace Detector
{
    struct BoundaryClassifierData
    {
        int imgWidth;
        int imgHeight;
        std::array<cv::Point2f, 4> corners;
        BoundaryClassifierData(
            int imgWidth,
            int imgHeight,
            std::array<cv::Point2f, 4> corners) : imgWidth(imgWidth), imgHeight(imgHeight), corners(corners){}
        ~BoundaryClassifierData(){}
    };

    class BoundaryClassifier : public IImageProcessing
    {
    public:
        BoundaryClassifier();
        virtual ~BoundaryClassifier();
        /**
         * @brief Classify image information based on the table corner
         * 
         * @param img unused
         * @param data pointer to a data structure containing the four corners of the table and the width, height of the ROI set
         * @return std::shared_ptr<void> structure containing pockets location pockets size and boudary location
         */
        virtual std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
    private:
        //data
    }; //BoundaryEnhancement
} // namespace Detector

#endif //BOUNDARYCLASSIFIER_HPP
