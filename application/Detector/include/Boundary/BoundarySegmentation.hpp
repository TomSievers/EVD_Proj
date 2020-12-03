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
        /**
         * @brief segment the image into needed objects
         * 
         * @param img image to segment on
         * @param data unused
         * @return std::shared_ptr<void> nullptr
         */
        virtual std::shared_ptr<void> process(cv::Mat& img, std::shared_ptr<void> data);
    private:
        void isolateTableBackground(cv::Mat& img);
    }; //BoundaryEnhancement
} // namespace Detector

#endif //BOUNDARYSEGMENTATION_HPP
