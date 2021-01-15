#include "include/Ball/BallDetector.hpp"
#include "include/Boundary/BoundaryDetector.hpp"
#include "include/IDetector.hpp"
#include <memory>
#include <iostream>
#include <chrono>

int main(int argc, char** argv)
{
    std::shared_ptr<Detector::Acquisition> cap =  std::make_shared<Detector::Acquisition>("../../Photos_pool_table/setup3_3.jpg");
    std::unique_ptr<Detector::IDetector> boundaryDetector = std::make_unique<Detector::BoundaryDetector>(cap);
    std::unique_ptr<Detector::IDetector> ballDetector = std::make_unique<Detector::BallDetector>(cap);

    boundaryDetector->getObjects(); // needed for setting ROI

    auto timePoint = std::chrono::system_clock::now();

    while(std::chrono::system_clock::now() < timePoint + std::chrono::milliseconds(1000))
    {
        // wait for ROI
    }
for(uint8_t i = 0; i < 10; i++){
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    std::vector<std::shared_ptr<Detector::Object>> objects = ballDetector->getObjects();

    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    std::cout << "detecting balls took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "milliseconds" << std::endl; 
}
}
