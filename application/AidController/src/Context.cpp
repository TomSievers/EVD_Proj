#include <include/Context.hpp>
#include <iostream>

Context::Context() : cur(nullptr), history(nullptr)
{

}

Context::~Context()
{

}

void Context::scheduleEvent(const UserInterface::Event& ev)
{
    std::cout << std::boolalpha << events.empty() << std::endl;
    EventContainer event;
    event.detector = Event::NONE;
    event.ui = ev;
    events.push(event);
    std::cout << std::boolalpha << events.empty() << std::endl;
}

void Context::scheduleEvent(const Event& ev)
{
    EventContainer event;
    event.detector = ev;
    event.ui = UserInterface::Event::NONE;
    events.push(event);
}