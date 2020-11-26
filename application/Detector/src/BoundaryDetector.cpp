#include <include/Boundary/BoundaryDetector.hpp>
#include <include/Boundary/BoundaryEnhancement.hpp>
#include <include/Boundary/BoundaryFeatureExtract.hpp>
#include <include/Boundary/BoundarySegmentation.hpp>
#include <opencv2/highgui.hpp>

namespace Detector
{
    BoundaryDetector::BoundaryDetector(std::shared_ptr<Acquisition> cap) : IDetector(cap)
    {
        processors.push_back(std::make_shared<BoundaryEnhancement>());
        processors.push_back(std::make_shared<BoundarySegmentation>());
        processors.push_back(std::make_shared<BoundaryFeatureExtract>());
    }

    BoundaryDetector::~BoundaryDetector()
    {

    }

    std::vector<std::shared_ptr<Object>> BoundaryDetector::getObjects()
    {
        std::string names[4] = {"acquisition", "ehance", "segment", "extract"};

        std::vector<std::shared_ptr<Object>> boundaries;
        int i = 0;
        for (auto& processor : processors)
        {
            cv::namedWindow(names[i], cv::WINDOW_KEEPRATIO);
            auto data_ptr = processor->process(img);
            cv::imshow(names[i], img);

            if(data_ptr != nullptr)
            {
                int width = 1000;
                int height = 500;
                int offset = width/50;
                std::shared_ptr<std::vector<cv::Point2f>> cornersVector = std::static_pointer_cast<std::vector<cv::Point2f>>(data_ptr);
                std::shared_ptr<Acquisition> capture = std::dynamic_pointer_cast<Acquisition>(processors[0]);
                std::array<cv::Point2f, 4> corners;
                std::copy_n(cornersVector->begin(), 4, corners.begin());
                capture->getCapture().setROI(corners, static_cast<float>(width), static_cast<float>(height));
                std::shared_ptr<Boundary> boundary = std::make_shared<Boundary>();
                boundary->corners = {
                    cv::Point(offset, offset), 
                    cv::Point(width-offset, offset), 
                    cv::Point(width-offset, height-offset), 
                    cv::Point(offset, height-offset)
                };
                boundary->pocketsLoc = {
                    cv::Point(offset, offset), 
                    cv::Point(width/2, 0),
                    cv::Point(width-offset, offset), 
                    cv::Point(width-offset, height-offset), 
                    cv::Point(width/2, height),
                    cv::Point(offset, height-offset)
                };
                boundary->pocketRad = width/50;
                boundaries.push_back(boundary);
            }
            ++i;
        }
        return boundaries;
    }
} // namespace Detector
