#include "include/Ball/BallDetector.hpp"
#include "include/Boundary/BoundaryDetector.hpp"
#include "include/IDetector.hpp"
#include <memory>
#include <iostream>
#include <chrono>

int main(int argc, char** argv)
{
    std::shared_ptr<Detector::Acquisition> cap =  std::make_shared<Detector::Acquisition>("../../Photos_pool_table/setup9_2.jpg");
    std::unique_ptr<Detector::IDetector> boundaryDetector = std::make_unique<Detector::BoundaryDetector>(cap);
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
    }
}