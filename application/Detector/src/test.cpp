#include <opencv2/highgui.hpp>
#include <include/Boundary/BoundaryDetector.hpp>
#include <include/Change/ChangeDetector.hpp>
#include <include/Ball/BallDetector.hpp>
#include <include/Cue/CueDetector.hpp>
#include <include/Acquisition.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <TrajectoryCalculator/include/TrajectoryCalculator.hpp>
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#define DEBUG
using namespace std::chrono_literals;

int main(int argc, char const *argv[])
{
    std::shared_ptr<Detector::Acquisition> cap =  std::make_shared<Detector::Acquisition>(0);

    std::shared_ptr<Detector::IDetector> detect = std::make_shared<Detector::BoundaryDetector>(cap);
    
    while(true)
    {
        auto bounds = detect->getObjects();
        /*cv::Mat frame = cap->getCapture().getFrame();

        if(!frame.empty())
        {
            cv::imshow("frame", frame);
        }*/
        if(bounds.size() != 0)
        {
            break;
        }
        cv::waitKey(1);
    }

    std::this_thread::sleep_for(500ms);
    
    std::shared_ptr<Detector::IDetector> changeDetect = std::make_shared<Detector::ChangeDetector>(cap);
    std::shared_ptr<Detector::IDetector> cueDetect = std::make_shared<Detector::CueDetector>(cap);
    std::shared_ptr<Detector::IDetector> ball = std::make_shared<Detector::BallDetector>(cap);

    auto balls = ball->getObjects();

    std::vector<cv::Point> realBalls;

    double avgRadius = 0;

    std::shared_ptr<Detector::BallObject> cueBall = nullptr;
    for(std::size_t i = 0; i < balls.size(); ++i)
    {
        auto realBall = std::dynamic_pointer_cast<Detector::BallObject>(balls[i]);
        realBalls.push_back(realBall->point);
        
        
        avgRadius += realBall->radius;
    }

    avgRadius /= realBalls.size();
    std::dynamic_pointer_cast<Detector::CueDetector>(cueDetect)->setBalls(realBalls, avgRadius);
    //auto cues = cueDetect->getObjects();
    while(true)
    {
        auto cues = cueDetect->getObjects();
        if(cv::waitKey(1) == 27)
        {
            break;
        }
    }    
    /*std::shared_ptr<Detector::Boundary> bound;
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
    }*/

    return 0;
}
