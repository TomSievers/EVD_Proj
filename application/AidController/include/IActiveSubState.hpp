#ifndef IACTIVESUBSTATE_HPP
#define IACTIVESUBSTATE_HPP

#include <include/IState.hpp>

class IActiveSubState : public IState
{
public:
    IActiveSubState(){};
	virtual ~IActiveSubState(){};
    virtual void onEntry(Controller& con) = 0;
    virtual void onDo(Controller& con) = 0;
    virtual void onExit(Controller& con) = 0;
    virtual bool handleEvent(Controller& con, const EventContainer& ev) = 0;
};

#endif //IACTIVESUBSTATE_HPP
