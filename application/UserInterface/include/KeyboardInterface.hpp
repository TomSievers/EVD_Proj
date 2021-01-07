#ifndef KEYBOARD_INTERFACE_HPP
#define KEYBOARD_INTERFACE_HPP

#include "IUserInterface.hpp"

namespace UserInterface
{
    class KeyboardInterface : public IUserinterface
    {
    public:
        /**
         * @brief Construct a new Keyboard Interface object
         * 
         */
        KeyboardInterface();
        /**
         * @brief Destroy the Keyboard Interface object
         * 
         */
        virtual ~KeyboardInterface();
        
        /**
         * @brief 
         * 
         */
        virtual void stop();
        virtual void update();
        
    private:
        bool active;
        std::thread thread;
    };
}

#endif