#include <opencv2/highgui.hpp>
#include <include/BoundaryDetector.hpp>

int main(int argc, char const *argv[])
{
    std::shared_ptr<Detector::IDetector> detect = std::make_shared<Detector::BoundaryDetector>();

    detect->getObjects();
    return 0;
}
