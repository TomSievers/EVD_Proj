#include <opencv2/highgui.hpp>
#include <include/Boundary/BoundaryDetector.hpp>
#include <include/Change/ChangeDetector.hpp>
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
    std::shared_ptr<Detector::Acquisition> cap =  std::make_shared<Detector::Acquisition>(0);
    cv::Mat img;


    std::shared_ptr<Detector::IDetector> detect = std::make_shared<Detector::BoundaryDetector>(cap);
    auto bounds = detect->getObjects();
    std::this_thread::sleep_for (std::chrono::milliseconds(1000));  
    std::shared_ptr<Detector::IDetector> changeDetect = std::make_shared<Detector::ChangeDetector>(cap);
    std::shared_ptr<Detector::IDetector> cueDetect = std::make_shared<Detector::CueDetector>(cap);

    cv::waitKey(0);
    auto cues = cueDetect->getObjects();

    

    std::shared_ptr<Detector::Boundary> bound;
    std::shared_ptr<Detector::CueObject> cue;

    bound = std::static_pointer_cast<Detector::Boundary>(bounds.at(0));

    for(int i = 0; i < bound->corners.size(); ++i)
    {
        std::cout << bound->corners[i].x << ", " << bound->corners[i].y << "\n";
    }

    try
    {
        bound = std::static_pointer_cast<Detector::Boundary>(bounds.at(0));
        cue = std::static_pointer_cast<Detector::CueObject>(cues.at(0));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    cv::destroyAllWindows();
    cv::Mat original;

    std::array<cv::Point2i, 4> points = {cv::Point2i(0, 0), cv::Point2i(500, 0), cv::Point2i(500, 400), cv::Point2i(0, 400)};
    (std::static_pointer_cast<Detector::ChangeDetector>(changeDetect))->setRoi(points);
    while(true)
    {
        
        cv::Mat frame = cap->getCapture().getFrame();

        if(!frame.empty())
        {
            auto changes = changeDetect->getObjects();
            std::shared_ptr<Detector::ChangeObject> changeObject = std::static_pointer_cast<Detector::ChangeObject>(changes.at(0));;

            if(changeObject->moving)
            {
                std::cout << "Is moving: Yes - " << changeObject->nonZero << "\n";
            }
            else
            {
                std::cout << "Is moving: No - " << changeObject->nonZero << "\n";
            }
            
            frame.copyTo(original);
            if(bound != nullptr)
            {
                for(auto pocket : bound->pocketsLoc)
                {
                    cv::circle(original, pocket, bound->pocketRad, cv::Scalar(0, 255, 0), 1);
                }

                for(int i = 0; i < bound->corners.size(); ++i)
                {
                    cv::line(original, bound->corners[i], bound->corners[i + 1], cv::Scalar(0, 0, 255), 2);
                }
            }
            if(cue != nullptr)
            {
                cv::circle(original, cv::Point(cue->endPoints[0].x, cue->endPoints[0].y), 8, cv::Scalar(255,0,0), cv::FILLED, cv::LINE_8);
            }

            cv::imshow("Original", original);
        }

        if(cv::waitKey(30) == 27)
        {
            cap->getCapture().stop();
            break;
        }
    }

    return 0;
}
