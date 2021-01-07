#include <include/Controller.hpp>

Controller::Controller(const std::string& cap)
{
    acquisition = std::make_shared<Detector::Acquisition>(cap);
}

Controller::Controller(int cap)
{
    acquisition = std::make_shared<Detector::Acquisition>(cap);
}

Controller::~Controller()
{

}

void Controller::changeState(const std::shared_ptr<IState> state)
{
    if(cur != nullptr)
    {
        cur->onExit(*this);
    }
    history = cur;
    cur = state;
    if(cur != nullptr)
    {
        cur->onEntry(*this);
    }
    
}

void Controller::run()
{
    if(cur != nullptr)
    {
        if(!events.empty())
        {
            if(cur->handleEvent(*this, events.front()))
            {
                events.pop();
            }
        }
        cur->onDo(*this);
    }
}

std::shared_ptr<Detector::Acquisition> Controller::getAcquisition()
{
    return acquisition;
}

void Controller::setDetector(const DetectorType& d, std::shared_ptr<Detector::IDetector> det)
{
    detectors[d] = det;
}

std::shared_ptr<Detector::IDetector> Controller::getDetector(const DetectorType& d)
{
    return detectors[d];
}

void Controller::setTrajectoryCalc(std::shared_ptr<TrajectoryCalculator::ITrajectory> traj)
{
    trajectoryCalc = traj;
}

std::shared_ptr<TrajectoryCalculator::ITrajectory> Controller::getTrajectoryCalc()
{
    return trajectoryCalc;
}