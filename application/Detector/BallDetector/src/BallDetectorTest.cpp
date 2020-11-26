#include "include/BallDetector.hpp"
#include "include/Object.hpp"
#include <memory>

int main(int argc, char** argv)
{
    std::unique_ptr<Detector::IDetector> ballDetector = std::make_unique<Detector::BallDetector>();

    std::vector<std::unique_ptr<Detector::Object>> objects = ballDetector->getObjects();
}