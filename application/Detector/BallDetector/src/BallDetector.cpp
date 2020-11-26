#include "include/BallDetector.hpp"
#include "include/BallAcquisition.hpp"
#include "include/BallEnhancement.hpp"
#include "include/BallSegmentation.hpp"
#include "include/BallFeatureExtraction.hpp"
#include "include/BallClassification.hpp"
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>

namespace Detector
{
    BallDetector::BallDetector()
    {
        imageProcessors.push_back(std::make_unique<BallAcquisition>());
        imageProcessors.push_back(std::make_unique<BallEnhancement>());
    }

    BallDetector::~BallDetector()
    {

    }

    std::vector<std::unique_ptr<Object>> BallDetector::getObjects()
    {
        cv::Mat image;
        std::vector<std::unique_ptr<Object>> ballObjects;
        for(const std::unique_ptr<IImageProcessing>& imageProcessor : imageProcessors)
        {
            image = imageProcessor->getImage(image);
            imageProcessor->updateImageData(image, ballObjects);
            cv::imshow("window", image);
            cv::waitKey(0);
        }
    }
}