#include <include/Boundary/BoundaryClassifier.hpp>
#include <include/IDetector.hpp>

namespace Detector
{
    BoundaryClassifier::BoundaryClassifier()
    {

    }

    BoundaryClassifier::~BoundaryClassifier()
    {
        
    }

    std::shared_ptr<void> BoundaryClassifier::process(cv::Mat& img, std::shared_ptr<void> data)
    {
        BoundaryClassifierData boundData = *std::static_pointer_cast<BoundaryClassifierData>(data);
        int offset = boundData.imgWidth/50;
        std::shared_ptr<Boundary> boundary = std::make_shared<Boundary>();

        boundary->corners = {
            cv::Point(offset, offset), 
            cv::Point(boundData.imgWidth-offset, offset), 
            cv::Point(boundData.imgWidth-offset, boundData.imgHeight-offset), 
            cv::Point(offset, boundData.imgHeight-offset)
        };

        boundary->pocketsLoc = {
            cv::Point(offset, offset), 
            cv::Point(boundData.imgWidth/2, 0),
            cv::Point(boundData.imgWidth-offset, offset), 
            cv::Point(boundData.imgWidth-offset, boundData.imgHeight-offset), 
            cv::Point(boundData.imgWidth/2, boundData.imgHeight),
            cv::Point(offset, boundData.imgHeight-offset)
        };

        boundary->pocketRad = boundData.imgWidth/50;
        
        return boundary;
    }
} // namespace Detector