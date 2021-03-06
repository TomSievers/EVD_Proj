#include <include/Controller.hpp>
#include <include/States.hpp>
#include <UserInterface/include/KeyboardInterface.hpp>
#include <opencv2/highgui.hpp>

using namespace std::chrono_literals;

int main(int argc, char const *argv[])
{
    
    std::shared_ptr<Controller> con = std::make_shared<Controller>(0);
    UserInterface::KeyboardInterface keyboard(std::bind(static_cast<void(Controller::*)(const UserInterface::Event&)>(&Controller::scheduleEvent), con, std::placeholders::_1));

    int key;
    int winKey = 0;
    while(true)
    {
        con->run();
        key = keyboard.getCurKey();
        winKey = cv::waitKey(1);
        if(key == 27 || winKey == 27)
        {
            keyboard.stop();
            break;
        }
    }
    return 0;
}
