#include <opencv2/highgui.hpp>
#include <include/Boundary/BoundaryDetector.hpp>
#include <include/Cue/CueDetector.hpp>
#include <include/Acquisition.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

int main(int argc, char const *argv[])
{
    std::shared_ptr<Detector::Acquisition> cap =  std::make_shared<Detector::Acquisition>("D:/EVD_Proj/Photos_pool_table/setup2_1.jpg");
    std::shared_ptr<Detector::IDetector> detect = std::make_shared<Detector::BoundaryDetector>(cap);
    std::shared_ptr<Detector::IDetector> cueDetect = std::make_shared<Detector::CueDetector>(cap);

    auto bounds = detect->getObjects();
    std::this_thread::sleep_for (std::chrono::milliseconds(100));  
    auto cues = cueDetect->getObjects();

    Detector::Boundary bound = *std::static_pointer_cast<Detector::Boundary>(bounds.at(0));
    Detector::CueObject cue = *std::static_pointer_cast<Detector::CueObject>(cues.at(0));
    while(true)
    {
        
        cv::Mat frame =  cap->getCapture().getFrame();
        if(!frame.empty())
        {
            for(auto pocket : bound.pocketsLoc)
            {
                cv::circle(frame, pocket, bound.pocketRad, cv::Scalar(0, 255, 0), 1);
            }

            for(int i = 0; i < bound.corners.size(); ++i)
            {
                cv::line(frame, bound.corners[i], bound.corners[i + 1], cv::Scalar(0, 0, 255), 2);
            }
            // std::cout << "x: "<<cue.center.x << " y:"<<cue.center.y <<std::endl;
            cv::circle(frame, cv::Point(cue.center.x, cue.center.y), 8, cv::Scalar(255,0,0), cv::FILLED, cv::LINE_8);
            cv::imshow("test", frame);
        }

        if(cv::waitKey(1) == 27)
        {
            cap->getCapture().stop();
            break;
        }
    }

    return 0;
}
