#include "include/KeyboardInterface.hpp"

void callback(const UserInterface::Event& e)
{
    switch (e)
    {
    case UserInterface::START:
        printf("START\r\n");
        break;
    case UserInterface::STOP:
        printf("STOP\r\n");
        break;
    case UserInterface::CALIBRATE:
        printf("CALIBRATE\r\n");
        break;
    default:
        break;
    }
}

int main()
{
    std::function<void(const UserInterface::Event&)> e;
    UserInterface::KeyboardInterface keyboardInterface(callback);
    int key = 0;
    int previousKey = -1;
    while(key != 27)
    {
        key = keyboardInterface.getCurKey();
        if(key != -1 && key != previousKey)
        {
            printf("char: %d\r\n", key);
            previousKey = key;
        }
    }
    return 0;
}
