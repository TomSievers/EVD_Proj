#include <include/IDetector.hpp>
#include <include/Cue/CueProcessing.hpp>
#include <opencv2/imgproc.hpp>
#define DEBUG
namespace Detector
{
    CueEnhancement::CueEnhancement()
    {

    }

    CueEnhancement::~CueEnhancement()
    {

    }

    std::shared_ptr<void> CueEnhancement::process(cv::Mat& image, std::shared_ptr<void> data)
    {
        cv::Mat temp;
        cv::bilateralFilter(image, temp, 7, 90 ,90);
        image = temp;
        blurImage(image);
        (void) data;
        return nullptr;
    }
    void CueEnhancement::blurImage(cv::Mat& image)
    {
        cv::GaussianBlur(image, image, cv::Size(7,7), 10, 0);
    }
}
