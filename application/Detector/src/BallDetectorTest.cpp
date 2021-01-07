#include "include/Ball/BallDetector.hpp"
#include "include/Boundary/BoundaryDetector.hpp"
#include "include/IDetector.hpp"
#include <memory>
#include <iostream>
#include <chrono>
#include <opencv2/highgui.hpp>

using namespace std::chrono_literals;

int main(int argc, char** argv)
{
    std::shared_ptr<Detector::Acquisition> cap =  std::make_shared<Detector::Acquisition>("../../Photos_pool_table/balls1.h264");
    cv::Mat img;
    std::array<cv::Point2f, 4> points = {cv::Point2f(500, 500), cv::Point2f(1200, 500), cv::Point2f(1200, 1600), cv::Point2f(500, 1600)};
    cap->getCapture().setROI(points, 700, 1100);
    std::this_thread::sleep_for(1s);
    while(true)
    {
        auto start = std::chrono::high_resolution_clock::now();
        img = cap->getCapture().getFrame();
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << std::endl;
        if(!img.empty())
        {
            cv::imshow("test", img);
        }

        int key = cv::waitKey(1);

        if(key == 27)
        {
            cap->getCapture().stop();
            break;
        } 
    }
    /*std::unique_ptr<Detector::IDetector> boundaryDetector = std::make_unique<Detector::BoundaryDetector>(cap);
    std::unique_ptr<Detector::IDetector> ballDetector = std::make_unique<Detector::BallDetector>(cap);

    boundaryDetector->getObjects(); // needed for setting ROI

    auto timePoint = std::chrono::system_clock::now();

    while(std::chrono::system_clock::now() < timePoint + std::chrono::milliseconds(1000))
    {
        // wait for ROI
    }

    std::vector<std::shared_ptr<Detector::Object>> objects = ballDetector->getObjects();

    for(std::shared_ptr<Detector::Object> objectPtr : objects)
    {
        std::cout << std::dynamic_pointer_cast<Detector::BallObject>(objectPtr)->point << " is of type  " 
                    << std::dynamic_pointer_cast<Detector::BallObject>(objectPtr)->ballType << " and has a white percentage of " 
                    << (uint16_t) std::dynamic_pointer_cast<Detector::BallObject>(objectPtr)->percentageWhite << std::endl;
    }*/
}