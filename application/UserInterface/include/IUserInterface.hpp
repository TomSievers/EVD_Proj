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
        CALIBRATE
    };

    class IUserinterface
    {
    public:
        /**
         * @brief Construct a new IUserinterface object
         * 
         */
        IUserinterface(std::function<void(const Event&)> callback) : callback(callback){}
        /**
         * @brief Destroy the IUserinterface object
         * 
         */
        virtual ~IUserinterface(){}
        /**
         * @brief 
         * 
         */
        virtual void stop() = 0;
    protected:
        virtual void update() = 0;
        std::function<void(const Event&)> callback;
        bool active;
        std::thread thread;
    };
}//namespace UserInterface

#endif