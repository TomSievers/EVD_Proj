#include <include/Boundary/BoundaryDetector.hpp>
#include <include/Boundary/BoundaryEnhancement.hpp>
#include <include/Boundary/BoundaryFeatureExtract.hpp>
#include <include/Boundary/BoundarySegmentation.hpp>
#include <include/Boundary/BoundaryClassifier.hpp>
#include <opencv2/highgui.hpp>

namespace Detector
{
    BoundaryDetector::BoundaryDetector(std::shared_ptr<Acquisition> cap) : IDetector(cap)
    {
        processors[ENHANCEMENT] = std::make_shared<BoundaryEnhancement>();
        processors[SEGMENTATION] = std::make_shared<BoundarySegmentation>();
        processors[FEATURE_EXTRACT] = std::make_shared<BoundaryFeatureExtract>();
        processors[CLASSIFICATION] = std::make_shared<BoundaryClassifier>();
    }

    BoundaryDetector::~BoundaryDetector()
    {

    }

    std::vector<std::shared_ptr<Object>> BoundaryDetector::getObjects()
    {
#ifdef DEBUG
        std::string names[5] = {"acquisition", "ehance", "segment", "extract", "classify"};
        int i = 0;
#endif

        std::vector<std::shared_ptr<Object>> boundaries;
        std::shared_ptr<void> data = nullptr;
        for (auto& processor : processors)
        {
            auto data_ptr = processor.second->process(img, data);
#ifdef DEBUG
            cv::namedWindow(names[i], cv::WINDOW_KEEPRATIO);
            //cv::imshow(names[i], img);
            ++i;
#endif

            if(data_ptr != nullptr)
            {
                switch (processor.first)
                {
                    case FEATURE_EXTRACT:
                    {
                        uint64_t width = 1000;
                        uint64_t height = width/2;
                        std::vector<cv::Point2f> cornersVector = *std::static_pointer_cast<std::vector<cv::Point2f>>(data_ptr);
                        std::shared_ptr<Acquisition> capture = std::dynamic_pointer_cast<Acquisition>(processors[ACQUISITION]);
                        std::array<cv::Point2f, 4> corners;
                        std::copy_n(cornersVector.begin(), 4, corners.begin());
                        capture->getCapture().setROI(corners, static_cast<float>(width), static_cast<float>(height));
                        data = std::make_shared<BoundaryClassifierData>(width, height, corners);
                        break;
                    }
                    
                    case CLASSIFICATION:
                    {
                        boundaries.push_back(std::static_pointer_cast<Object>(data_ptr));
                        break;
                    }
                    default:
                    {
                        throw(std::runtime_error("Unexpected data pointer returned by processor"));
                        break;
                    }
                }
            }
        }

        return boundaries;
    }
} // namespace Detector
