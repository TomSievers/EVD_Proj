#ifndef IUSERINTERFACE_HPP
#define IUSERINTERFACE_HPP

#include <functional>
#include <thread>

namespace UserInterface
{
    enum Event
    {
        START,
        STOP,
        CALIBRATE,
        NEXT_CALIBRATION,
        NONE
    };

    class IUserinterface
    {
    public:
        /**
         * @brief Construct a new IUserinterface object
         * 
         * @param callback 
         */
        IUserinterface(std::function<void(const Event&)> callback) : callback(callback){}
        /**
         * @brief Destroy the IUserinterface object
         * 
         */
        virtual ~IUserinterface(){}
        /**
         * @brief stop reading keyboard inputs
         * 
         */
        virtual void stop() = 0;
    protected:
        /**
         * @brief check for key presses
         * if the A, C, S or ESC key has been pressed an action is required
         * 
         */
        virtual void update() = 0;
        std::function<void(const Event&)> callback;
        bool active;
        std::thread thread;
    };
}//namespace UserInterface

#endif