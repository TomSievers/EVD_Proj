#include <opencv2/highgui.hpp>
#include <include/Boundary/BoundaryDetector.hpp>
#include <include/Ball/BallDetector.hpp>
#include <include/Cue/CueDetector.hpp>
#include <include/Acquisition.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <TrajectoryCalculator/include/TrajectoryCalculator.hpp>
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

using namespace std::chrono_literals;

int main(int argc, char const *argv[])
{
    std::shared_ptr<Detector::Acquisition> cap =  std::make_shared<Detector::Acquisition>("../../Photos_pool_table/balls1.h264");
    cv::Mat img;
    // skip first frames because a lot darker
    for(auto i = 0; i < 30; ++i)
    {
        cap->process(img, nullptr);
        std::this_thread::sleep_for(100ms);
    }
    
    std::shared_ptr<Detector::IDetector> detect = std::make_shared<Detector::BoundaryDetector>(cap);
    std::shared_ptr<Detector::IDetector> cueDetect = std::make_shared<Detector::CueDetector>(cap);
    std::unique_ptr<Detector::IDetector> ballDetector = std::make_unique<Detector::BallDetector>(cap);

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
        std::cerr << "error: " << e.what() << '\n';
    }
    
    while(true)
    {
        
        cv::Mat frame =  cap->getCapture().getFrame();
        if(!frame.empty())
        {
            std::vector<std::shared_ptr<Detector::Object>> objects = ballDetector->getObjects();

            /*for(std::shared_ptr<Detector::Object> objectPtr : objects)
            {
                std::cout << std::dynamic_pointer_cast<Detector::BallObject>(objectPtr)->point << " is of type  " 
                            << std::dynamic_pointer_cast<Detector::BallObject>(objectPtr)->ballType << " and has a white percentage of " 
                            << (uint16_t) std::dynamic_pointer_cast<Detector::BallObject>(objectPtr)->percentageWhite << std::endl;
            }*/

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
