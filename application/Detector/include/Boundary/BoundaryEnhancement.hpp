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
        /**
         * @brief Ehance the image using a blur filter
         * 
         * @param img image to enhance
         * @param data unused
         * @return std::shared_ptr<void> nullptr
         */
        virtual std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
    private:
        //data
    }; //BoundaryEnhancement
} // namespace Detector

#endif //BOUNDARYENHANCEMENT_HPP
