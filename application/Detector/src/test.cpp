#include <opencv2/highgui.hpp>
#include <include/BoundaryDetector.hpp>
#include <include/Acquisition.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, char const *argv[])
{
    std::shared_ptr<Detector::Acquisition> cap =  std::make_shared<Detector::Acquisition>("../../Photos_pool_table/setup9_3.jpg");
    //std::shared_ptr<Detector::IDetector> detect = std::make_shared<Detector::BoundaryDetector>(cap);

    //auto bounds = detect->getObjects();

    //Detector::Boundary bound = *std::static_pointer_cast<Detector::Boundary>(bounds.at(0));

    while(true)
    {
        
        cv::Mat frame =  cap->getCapture().getFrame();
        if(!frame.empty())
        {
            /*for(auto pocket : bound.pocketsLoc)
            {
                cv::circle(frame, pocket, bound.pocketRad, cv::Scalar(0, 255, 0), 1);
            }*/
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
