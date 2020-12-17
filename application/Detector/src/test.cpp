#include <opencv2/highgui.hpp>
#include <include/Boundary/BoundaryDetector.hpp>
#include <include/Cue/CueDetector.hpp>
#include <include/Acquisition.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <TrajectoryCalculator/include/TrajectoryCalculator.hpp>
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

int main(int argc, char const *argv[])
{
    std::shared_ptr<Detector::Acquisition> cap =  std::make_shared<Detector::Acquisition>("../../Photos_pool_table/setup2_1.jpg");
    std::shared_ptr<Detector::IDetector> detect = std::make_shared<Detector::BoundaryDetector>(cap);
    std::shared_ptr<Detector::IDetector> cueDetect = std::make_shared<Detector::CueDetector>(cap);

    auto bounds = detect->getObjects();
    std::this_thread::sleep_for (std::chrono::milliseconds(1000));  
    auto cues = cueDetect->getObjects();

    std::shared_ptr<Detector::Boundary> bound;
    std::shared_ptr<Detector::CueObject> cue;

    try
    {
        bound = std::static_pointer_cast<Detector::Boundary>(bounds.at(0));
        cue = std::static_pointer_cast<Detector::CueObject>(cues.at(0));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
        while(true)
    {
        
        cv::Mat frame =  cap->getCapture().getFrame();
        if(!frame.empty())
        {
            if(bound != nullptr)
            {
                for(auto pocket : bound->pocketsLoc)
                {
                    cv::circle(frame, pocket, bound->pocketRad, cv::Scalar(0, 255, 0), 1);
                }

                for(int i = 0; i < bound->corners.size(); ++i)
                {
                    cv::line(frame, bound->corners[i], bound->corners[i + 1], cv::Scalar(0, 0, 255), 2);
                }
            }
            if(cue != nullptr)
            {
                cv::circle(frame, cv::Point(cue->endPoints[0].x, cue->endPoints[0].y), 8, cv::Scalar(255,0,0), cv::FILLED, cv::LINE_8);
                cv::imshow("test", frame);
            }
        }

        if(cv::waitKey(1) == 27)
        {
            cap->getCapture().stop();
            break;
        }
    }

    return 0;
}
