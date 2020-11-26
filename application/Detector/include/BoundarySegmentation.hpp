#ifndef BOUNDARYSEGMENTATION_HPP
#define BOUNDARYSEGMENTATION_HPP

#include <include/IImageProcessing.hpp>

namespace Detector
{
    class BoundarySegmentation : public IImageProcessing
    {
    public:
        BoundarySegmentation();
        virtual ~BoundarySegmentation();
        virtual std::shared_ptr<void> process(cv::Mat& img);
    private:
        //data
    }; //BoundaryEnhancement
} // namespace Detector

#endif //BOUNDARYSEGMENTATION_HPP
