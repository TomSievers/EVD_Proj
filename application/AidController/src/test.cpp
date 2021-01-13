#include <include/Controller.hpp>
#include <include/States.hpp>
#include <UserInterface/include/KeyboardInterface.hpp>
#include <opencv2/highgui.hpp>

using namespace std::chrono_literals;

int main(int argc, char const *argv[])
{
    
    std::shared_ptr<Controller> con = std::make_shared<Controller>("../../Photos_pool_table/setup1_3.jpg");
    UserInterface::KeyboardInterface keyboard(std::bind(static_cast<void(Controller::*)(const UserInterface::Event&)>(&Controller::scheduleEvent), con, std::placeholders::_1));

    int key;
    int winKey;
    while(true)
    {
        con->run();
        key = keyboard.getCurKey();
        winKey = cv::waitKey(1);
        if(key == 27 || winKey == 27)
        {
            break;
        }
    }
    return 0;
}
