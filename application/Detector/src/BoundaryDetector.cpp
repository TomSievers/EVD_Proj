#include <include/BoundaryDetector.hpp>
#include <include/MockAcquisition.hpp>
#include <include/Acquisition.hpp>
#include <include/BoundaryEnhancement.hpp>
#include <include/BoundaryFeatureExtract.hpp>
#include <include/BoundarySegmentation.hpp>
#include <iostream>
#include <opencv2/highgui.hpp>

namespace Detector
{
    BoundaryDetector::BoundaryDetector()
    {
        processors.push_back(std::make_shared<MockAcquisition>());
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
        int i = 0;
        for (auto& processor : processors)
        {
            auto data_ptr = processor->process(img);
            cv::imshow(names[i], img);

            if(data_ptr != nullptr)
            {
                std::cout << "reached not nullptr" << std::endl;
            }
            cv::waitKey(0);
            ++i;
        }
        

    }
} // namespace Detector
