#ifndef ISTATE_HPP
#define ISTATE_HPP

struct EventContainer;
class Controller;
#include <memory>

class IState
{
public:
	IState(){};
	virtual ~IState(){};
    virtual void onEntry(Controller& con) = 0;
    virtual void onDo(Controller& con) = 0;
    virtual void onExit(Controller& con) = 0;
    virtual bool handleEvent(Controller& con, const EventContainer& ev) = 0;
private:
	//data
}; //IState

#endif //ISTATE_HPP
