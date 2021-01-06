#include "include/Ball/BallDetector.hpp"
#include "include/Boundary/BoundaryDetector.hpp"
#include "include/IDetector.hpp"
#include <memory>
#include <iostream>
#include <chrono>
#include <opencv2/highgui.hpp>
#ifdef __linux__
#include <dirent.h>
#endif

using namespace std::chrono_literals;

int main(int argc, char** argv)
{
    std::string path = "../../Photos_pool_table";
    DIR* dir = opendir(path.c_str());
    if(dir != NULL)
    {
        dirent* entry = readdir(dir);
        while(entry != NULL)
        {
            if(strncmp("setup", entry->d_name, 5) == 0)
            {
                std::string file = path + "/" + entry->d_name;
                std::cout << file << std::endl;
                std::shared_ptr<Detector::Acquisition> cap = std::make_shared<Detector::Acquisition>(file);
                std::unique_ptr<Detector::IDetector> boundaryDetector = std::make_unique<Detector::BoundaryDetector>(cap);
                std::unique_ptr<Detector::IDetector> ballDetector = std::make_unique<Detector::BallDetector>(cap);
                
                boundaryDetector->getObjects(); // needed for setting ROI
                
                std::this_thread::sleep_for(100ms);

                auto start =  std::chrono::high_resolution_clock::now();
                std::vector<std::shared_ptr<Detector::Object>> objects = ballDetector->getObjects();
                auto end =  std::chrono::high_resolution_clock::now();

                std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

                /*for(std::shared_ptr<Detector::Object> objectPtr : objects)
                {
                    std::cout << std::dynamic_pointer_cast<Detector::BallObject>(objectPtr)->point << " is of type  " 
                                << std::dynamic_pointer_cast<Detector::BallObject>(objectPtr)->ballType << " and has a white percentage of " 
                                << (uint16_t) std::dynamic_pointer_cast<Detector::BallObject>(objectPtr)->percentageWhite << std::endl;
                }*/

                for(std::size_t i = 0; i < objects.size(); ++i)
                {
                    if(!ballObjectPtr->at(i)->ballType) // no cue ball
                        cv::circle(EnhancedImage, ballObjectPtr->at(i)->point, ballObjectPtr->at(i)->radius, cv::Scalar(255, 0, 0), 1);
                    else
                        cv::circle(EnhancedImage, ballObjectPtr->at(i)->point, ballObjectPtr->at(i)->radius, cv::Scalar(0, 0, 255), -1);
                }
                cv::imshow("results", EnhancedImage);
                cv::waitKey(10000);
            }

            //std::this_thread::sleep_for(5s);
            entry = readdir(dir);
        }
    }

    

    
}