#include "include/Configure/TableConfDetector.hpp"
#include "include/Configure/BallConfDetector.hpp"
#include "include/Configure/CueConfDetector.hpp"
#include "include/IDetector.hpp"
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
    std::shared_ptr<Detector::Acquisition> cap =  std::make_shared<Detector::Acquisition>("empty_table.jpg");
    std::shared_ptr<Detector::Acquisition> capCueBall = std::make_shared<Detector::Acquisition>("table_with_ball.jpg");
    std::shared_ptr<Detector::Acquisition> capCue = std::make_shared<Detector::Acquisition>("cue.png");
    
    std::shared_ptr<Detector::IDetector> tableConfigDetector = std::make_shared<Detector::TableConfDetector>(cap);
    std::shared_ptr<Detector::IDetector> cueBallConfigDetector = std::make_shared<Detector::BallConfDetector>(capCueBall);
    std::shared_ptr<Detector::IDetector> cueConfigDetector = std::make_shared<Detector::CueConfDetector>(capCue);

    std::this_thread::sleep_for(500ms); // wait on capture thread

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

    cv::Mat image = cv::imread("empty_table.jpg");
    cv::Mat hsvImage;
    cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);

    cv::Mat cueBallImage = cv::imread("table_with_ball.jpg");
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
    cv::imshow("image in range cue", inRangeImageCue);
    cv::waitKey(0);

    return 0;
}