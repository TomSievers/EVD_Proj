#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <include/Context.hpp>
#include <map>
#include <Detector/include/IDetector.hpp>
#include <TrajectoryCalculator/include/ITrajectory.hpp>
#include <Visualizer/include/IVisual.hpp>

enum DetectorType
{
    CUE,
    BALL,
    BOUNDARY
};

class Controller : public Context
{
public:
	Controller(const std::string& cap);
    Controller(int cap);
	virtual ~Controller();
    virtual void changeState(const std::shared_ptr<IState> state);
    virtual void changeState();
    virtual void run();
    void setDetector(const DetectorType& d, std::shared_ptr<Detector::IDetector> det);
    std::shared_ptr<Detector::IDetector> getDetector(const DetectorType& d);
    void setTrajectoryCalc(std::shared_ptr<TrajectoryCalculator::ITrajectory> traj);
    std::shared_ptr<TrajectoryCalculator::ITrajectory> getTrajectoryCalc();
    std::shared_ptr<Detector::Acquisition> getAcquisition();
    void setVisualizer(std::shared_ptr<Visualizer::IVisual> vis);
    std::shared_ptr<Visualizer::IVisual> getVisualizer();
    
private:
	std::map<DetectorType, std::shared_ptr<Detector::IDetector>> detectors;
    std::shared_ptr<Detector::Acquisition> acquisition;
    std::shared_ptr<TrajectoryCalculator::ITrajectory> trajectoryCalc;
    std::shared_ptr<Visualizer::IVisual> visualizer;
}; //Controller

#endif //CONTROLLER_HPP
