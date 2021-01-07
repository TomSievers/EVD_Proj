#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <queue>
#include <memory>
#include <include/IState.hpp>

//Stub for UI
namespace UserInterface
{
    enum Event
    {
        NONE,
        START,
        STOP,
        CALIBRATE
    };

} // namespace UserInterface



enum Event
{
    NONE,
    STEADY,
    MOVING
};

struct EventContainer
{
    UserInterface::Event ui;
    Event detector;
};

class Context
{
public:
	Context();
	virtual ~Context();
    void scheduleEvent(const UserInterface::Event& ev);
    void scheduleEvent(const Event& ev);
    virtual void changeState(const std::shared_ptr<IState> state) = 0;
    virtual void run() = 0;
protected:
    std::queue<EventContainer> events;
    std::shared_ptr<IState> cur;
    std::shared_ptr<IState> history;
}; //Context

#endif //CONTEXT_HPP
