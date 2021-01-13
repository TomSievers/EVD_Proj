#include "include/Configure/TableConfDetector.hpp"
#include "include/Configure/BallConfDetector.hpp"
#include "include/Configure/CueConfDetector.hpp"
#include "include/Cue/CueDetector.hpp"
#include "include/Boundary/BoundaryDetector.hpp"
#include "include/IDetector.hpp"
#include "include/Configure/Configuration.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <memory>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

int main(int argc, char** argv)
{
    std::shared_ptr<Detector::Acquisition> cap =  std::make_shared<Detector::Acquisition>("empty_table.png");
    std::shared_ptr<Detector::Acquisition> capCueBall = std::make_shared<Detector::Acquisition>("table_with_ball.png");
    std::shared_ptr<Detector::Acquisition> capCue = std::make_shared<Detector::Acquisition>("cue.png");
    
    std::shared_ptr<Detector::IDetector> tableConfigDetector = std::make_shared<Detector::TableConfDetector>(cap);
    std::shared_ptr<Detector::IDetector> cueBallConfigDetector = std::make_shared<Detector::BallConfDetector>(capCueBall);
    std::shared_ptr<Detector::IDetector> cueConfigDetector = std::make_shared<Detector::CueConfDetector>(capCue);

    std::this_thread::sleep_for(1000ms); // wait on capture thread

    std::vector<std::shared_ptr<Detector::Object>> tableColor = tableConfigDetector->getObjects();
    
    std::dynamic_pointer_cast<Detector::BallConfDetector>(cueBallConfigDetector)->setCurrentConfig(tableColor.at(0));
    std::vector<std::shared_ptr<Detector::Object>> cueBallColor = cueBallConfigDetector->getObjects();

    std::dynamic_pointer_cast<Detector::CueConfDetector>(cueConfigDetector)->setCurrentConfig(cueBallColor.at(0));
    std::vector<std::shared_ptr<Detector::Object>> cueColor = cueConfigDetector->getObjects();
    
    cap->getCapture().stop();
    capCueBall->getCapture().stop();
    capCue->getCapture().stop();

    for(std::shared_ptr<Detector::Object> objectPtr : tableColor)
    {
        std::cout << "table color hsv min: " << std::dynamic_pointer_cast<Detector::Config>(objectPtr)->tableColorMin[0] << " " << std::dynamic_pointer_cast<Detector::Config>(objectPtr)->tableColorMin[1] << " " << std::dynamic_pointer_cast<Detector::Config>(objectPtr)->tableColorMin[0] << std::endl;
        std::cout << "table color hsv max: " << std::dynamic_pointer_cast<Detector::Config>(objectPtr)->tableColorMax[0] << " " << std::dynamic_pointer_cast<Detector::Config>(objectPtr)->tableColorMax[1] << " " << std::dynamic_pointer_cast<Detector::Config>(objectPtr)->tableColorMax[0] << std::endl;
    }

    cv::Mat image = cv::imread("empty_table.png");
    cv::Mat hsvImage;
    cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);

    cv::Mat cueBallImage = cv::imread("table_with_ball.png");
    cv::Mat hsvcueBallImage;
    cv::cvtColor(cueBallImage, hsvcueBallImage, cv::COLOR_BGR2HSV);

    cv::Mat cueImage = cv::imread("cue.png");
    cv::Mat hsvCueImage;
    cv::cvtColor(cueImage, hsvCueImage, cv::COLOR_BGR2HSV);

    cv::Mat inRangeImage;
    cv::Mat inRangeImageCueBall;
    cv::Mat inRangeImageCue;

    cv::Scalar hsvMin = std::dynamic_pointer_cast<Detector::Config>(tableColor.at(0))->tableColorMin;
    cv::Scalar hsvMax = std::dynamic_pointer_cast<Detector::Config>(tableColor.at(0))->tableColorMax;    
    cv::Scalar hsvMinCueBall = std::dynamic_pointer_cast<Detector::Config>(cueBallColor.at(0))->cueBallColorMin;  
    cv::Scalar hsvMaxCueBall = std::dynamic_pointer_cast<Detector::Config>(cueBallColor.at(0))->cueBallColorMax;
    cv::Scalar hsvMinCue = std::dynamic_pointer_cast<Detector::Config>(cueColor.at(0))->cueColorMin;  
    cv::Scalar hsvMaxCue = std::dynamic_pointer_cast<Detector::Config>(cueColor.at(0))->cueColorMax;

    cv::inRange(hsvImage, hsvMin, hsvMax, inRangeImage);
    cv::inRange(hsvcueBallImage, hsvMinCueBall, hsvMaxCueBall, inRangeImageCueBall);
    cv::inRange(hsvCueImage, hsvMinCue, hsvMaxCue, inRangeImageCue);

    cv::imshow("image in range", inRangeImage);
    cv::imshow("image in range cue ball", inRangeImageCueBall);
    cv::imshow("image in range cue", inRangeImageCue); // empty because of preprocessing
    cv::waitKey(0);

    Configuration::getInstance().setConfig(std::dynamic_pointer_cast<Detector::Config>(cueColor.at(0)));


    // read in video stream
    std::shared_ptr<Detector::Acquisition> capture =  std::make_shared<Detector::Acquisition>("play.h264");
    cv::Mat captureImg;
    // skip first frames because a lot darker
    for(auto i = 0; i < 30; ++i)
    {
        capture->process(captureImg, nullptr);
        std::this_thread::sleep_for(100ms);
    }

    std::shared_ptr<Detector::IDetector> detect = std::make_shared<Detector::BoundaryDetector>(capture);
    std::shared_ptr<Detector::IDetector> cueDetect = std::make_shared<Detector::CueDetector>(capture);



    std::vector<std::shared_ptr<Detector::Object>> bounds;

    //do
   // {
     //   bounds = detect->getObjects();
    //} while(bounds.size() == 0);

    std::shared_ptr<Detector::Boundary> bound;// = std::static_pointer_cast<Detector::Boundary>(bounds.at(0));

    std::this_thread::sleep_for (std::chrono::milliseconds(1000));  
    
    // keep asking frames
    while(true)
    {
        auto cues = cueDetect->getObjects();

        std::shared_ptr<Detector::CueObject> cue;

        try
        {
            cue = std::static_pointer_cast<Detector::CueObject>(cues.at(0));
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }

        cv::Mat frame = capture->getCapture().getFrame();
        
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
        }

        cv::imshow("test", frame);
        cv::waitKey(0);
    }
    return 0;
}