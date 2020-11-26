#ifndef BOUNDARYENHANCEMENT_HPP
#define BOUNDARYENHANCEMENT_HPP

#include <include/IImageProcessing.hpp>

namespace Detector
{
    class BoundaryEnhancement : public IImageProcessing
    {
    public:
        BoundaryEnhancement();
        virtual ~BoundaryEnhancement();
        virtual std::shared_ptr<void> process(cv::Mat& img);
    private:
        //data
    }; //BoundaryEnhancement
} // namespace Detector

#endif //BOUNDARYENHANCEMENT_HPP
