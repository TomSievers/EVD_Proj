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
         * @param callback 
         */
        KeyboardInterface(std::function<void(const Event&)> callback);
        /**
         * @brief Destroy the Keyboard Interface object
         * 
         */
        virtual ~KeyboardInterface();
        
        /**
         * @brief stop checking for keyboard input
         * 
         */
        virtual void stop();
        /**
         * @brief Get the the last key that is pressed
         * 
         * @return int curKey
         */
        int getCurKey();
    protected:
        /**
         * @brief check for keyboard input
         * if the A, C, S or ESC key has been pressed an action is required
         * 
         */
        virtual void update();
    private:
        int curKey;
    };
}

#endif