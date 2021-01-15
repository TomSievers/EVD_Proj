#ifndef ACTIVESUBSTATES_HPP
#define ACTIVESUBSTATES_HPP

#include <include/IActiveSubState.hpp>

class Detecting : public IActiveSubState
{
public:
    Detecting();
	virtual ~Detecting();
    virtual void onEntry(Controller& con);
    virtual void onDo(Controller& con);
    virtual void onExit(Controller& con);
    virtual bool handleEvent(Controller& con, const EventContainer& ev);
};

class Waiting : public IActiveSubState
{
public:
    Waiting();
	virtual ~Waiting();
    virtual void onEntry(Controller& con);
    virtual void onDo(Controller& con);
    virtual void onExit(Controller& con);
    virtual bool handleEvent(Controller& con, const EventContainer& ev);
};

#endif //ACTIVESUBSTATES_HPP
