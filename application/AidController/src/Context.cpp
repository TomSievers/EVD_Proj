#include <include/Context.hpp>

Context::Context() : cur(nullptr), history(nullptr)
{

}

Context::~Context()
{

}

void Context::scheduleEvent(const UserInterface::Event& ev)
{
    EventContainer event;
    event.detector = Event::NONE;
    event.ui = ev;
    events.push(event);
}

void Context::scheduleEvent(const Event& ev)
{
    EventContainer event;
    event.detector = ev;
    event.ui = UserInterface::Event::NONE;
    events.push(event);
}