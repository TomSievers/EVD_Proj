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
        IUserinterface(){}
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
        
        std::function<void(Event&)> IUserinterfaceFunc;

    protected:
        virtual void update() = 0;
        std::function<void(Event&)> callback;

    private:
        bool active;
        std::thread thread;
    };
}//namespace UserInterface

#endif