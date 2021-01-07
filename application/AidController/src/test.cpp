#include <include/Controller.hpp>
#include <include/States.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

int main(int argc, char const *argv[])
{
    Controller con("../../Photos_pool_table/setup1_3.jpg");
    while(true)
    {
        con.run();
        int key = cv::waitKey(0);
        if(key == 27)
        {
            break;
        } else if( key != -1) {
            if(key == 97)
            {
                con.scheduleEvent(UserInterface::START);
            } else if(key == 115)
            {
                con.scheduleEvent(UserInterface::STOP);
            } else if(key == 99)
            {
                con.scheduleEvent(UserInterface::CALIBRATE);
            }
            std::cout << key << std::endl;
        }
    }
    return 0;
}
