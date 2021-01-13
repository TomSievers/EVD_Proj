#ifndef KEYBOARD_INTERFACE_HPP
#define KEYBOARD_INTERFACE_HPP

#include "IUserInterface.hpp"

#if defined(_WIN32)
#include <windows.h>
#endif

namespace UserInterface
{
    class KeyboardInterface : public IUserinterface
    {
    public:
        /**
         * @brief Construct a new Keyboard Interface object
         * 
         */
        KeyboardInterface(std::function<void(const Event&)> callback);
        /**
         * @brief Destroy the Keyboard Interface object
         * 
         */
        virtual ~KeyboardInterface();
        
        /**
         * @brief 
         * 
         */
        virtual void update();
        virtual void stop();
        char getCurKey();
        
    private:
        bool callbackEnable;
        int curKey;
    };
}

#endif