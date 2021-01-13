#include <include/States.hpp>
#include <Detector/include/Boundary/BoundaryDetector.hpp>
#include <Detector/include/Ball/BallDetector.hpp>
#include <Detector/include/Cue/CueDetector.hpp>
#include <TrajectoryCalculator/include/TrajectoryCalculator.hpp>
#include <Detector/include/Change/ChangeDetector.hpp>
#include <thread>
#include <chrono>
#include <Visualizer/include/ObjectVisualizer.hpp>
#include <iostream>

using namespace std::chrono_literals;

Setup::Setup()
{
    
}

Setup::~Setup()
{
    
}

void Setup::onEntry(Controller& con)
{
    
    con.setDetector(BOUNDARY, std::make_shared<Detector::BoundaryDetector>(con.getAcquisition()));
    con.setDetector(BALL, std::make_shared<Detector::BallDetector>(con.getAcquisition()));
    con.setDetector(CUE, std::make_shared<Detector::CueDetector>(con.getAcquisition()));
    con.setDetector(CHANGE, std::make_shared<Detector::ChangeDetector>(con.getAcquisition()));
    con.setTrajectoryCalc(std::make_shared<TrajectoryCalculator::TrajectoryCalculator>());
#if defined(__linux__) && defined(HAVE_CAIRO)
    con.setVisualizer(std::make_shared<Visualizer::ObjectVisualizer>(CAIRO_FORMAT_RGB16_565, cv::Point(0, 0), cv::Point(1000, 500)));
#else
    con.setVisualizer(std::make_shared<Visualizer::ObjectVisualizer>(cv::Point(0, 0), cv::Point(1000, 500)));
#endif

}

void Setup::onDo(Controller& con)
{
    std::shared_ptr<Detector::Boundary> boundary;
    while(boundary == nullptr)
    {
        auto objects = con.getDetector(BOUNDARY)->getObjects();
        if(!objects.empty())
        {
            boundary = std::dynamic_pointer_cast<Detector::Boundary>(objects.at(0));
        }
    }

    con.getTrajectoryCalc()->setPocketRadius(static_cast<uint16_t>(round(boundary->pocketRad)));
    con.getTrajectoryCalc()->setTableCorners(boundary->corners);
    con.changeState(std::make_shared<Inactive>());
}

void Setup::onExit(Controller& con)
{
    
    std::this_thread::sleep_for(100ms);
}

bool Setup::handleEvent(Controller& con, const EventContainer& ev)
{
    return false;
}

Inactive::Inactive()
{
    
}

Inactive::~Inactive()
{   
    
}

void Inactive::onEntry(Controller& con)
{
    
}

void Inactive::onDo(Controller& con)
{

}

void Inactive::onExit(Controller& con)
{
    
}   

bool Inactive::handleEvent(Controller& con, const EventContainer& ev)
{
    switch (ev.ui)
    {
    case UserInterface::Event::START:
        {
            con.changeState(std::make_shared<Active>());
        }
        break;
    case UserInterface::Event::CALIBRATE:
        {
            con.changeState(std::make_shared<Calibrate>());
        }
        break;
    
    default:
        break;
    }
    return true;
}

Active::Active()
{
    
}

Active::~Active()
{
    
}

void Active::onEntry(Controller& con)
{
    
    substate = std::make_shared<Waiting>();
}

void Active::onDo(Controller& con)
{
    if(substate != nullptr)
    {
        substate->onDo(con);
    }
}

void Active::onExit(Controller& con)
{
    
}

bool Active::handleEvent(Controller& con, const EventContainer& ev)
{
    switch (ev.ui)
    {
    case UserInterface::Event::STOP:
        {
            con.changeState(std::make_shared<Inactive>());
        }
        break;
    case UserInterface::Event::CALIBRATE:
        {
            con.changeState(std::make_shared<Calibrate>());
        }
        break;
    
    default:
        break;
    }

    switch (ev.detector)
    {
    case Event::STEADY:
        {   
            substate = std::make_shared<Detecting>();
        }
        break;
    case Event::MOVING:
        {   
            substate = std::make_shared<Waiting>();
        }
        break;
    
    default:
        break;
    }
    return true;
}

Calibrate::Calibrate()
{
    
}

Calibrate::~Calibrate()
{   
    
}

void Calibrate::onEntry(Controller& con)
{
    
}

void Calibrate::onDo(Controller& con)
{
    con.changeState();
}

void Calibrate::onExit(Controller& con)
{
    
}

bool Calibrate::handleEvent(Controller& con, const EventContainer& ev)
{
    return true;
}