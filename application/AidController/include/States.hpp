#ifndef STATES_HPP
#define STATES_HPP

#include <include/IState.hpp>
#include <include/Controller.hpp>
#include <include/ActiveSubStates.hpp>

class Setup : public IState
{
public:
	Setup();
	virtual ~Setup();
    virtual void onEntry(Controller& con);
    virtual void onDo(Controller& con);
    virtual void onExit(Controller& con);
    virtual bool handleEvent(Controller& con, const EventContainer& ev);
private:
	//data
};


class Inactive : public IState
{
public:
	Inactive();
	virtual ~Inactive();
    virtual void onEntry(Controller& con);
    virtual void onDo(Controller& con);
    virtual void onExit(Controller& con);
    virtual bool handleEvent(Controller& con, const EventContainer& ev);
private:
	//data
};

class Active : public IState
{
public:
	Active();
	virtual ~Active();
    virtual void onEntry(Controller& con);
    virtual void onDo(Controller& con);
    virtual void onExit(Controller& con);
    virtual bool handleEvent(Controller& con, const EventContainer& ev);
private:
	std::shared_ptr<IActiveSubState> substate;
};

class Calibrate : public IState
{
public:
	Calibrate();
	virtual ~Calibrate();
    virtual void onEntry(Controller& con);
    virtual void onDo(Controller& con);
    virtual void onExit(Controller& con);
    virtual bool handleEvent(Controller& con, const EventContainer& ev);
private:
    std::shared_ptr<Detector::Object> config;
    bool nextCalibration;
    DetectorType currentDetector;
	//data
};

#endif //STATES_HPP
