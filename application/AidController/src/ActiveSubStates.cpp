#include <include/ActiveSubStates.hpp>
#include <include/Controller.hpp>
#include <opencv2/core.hpp>
#include <vector>

Detecting::Detecting()
{

}

Detecting::~Detecting()
{

}

void Detecting::onEntry(Controller& con)
{

}

void Detecting::onDo(Controller& con)
{
    std::vector<std::shared_ptr<Detector::Object>> balls;
    while(balls.empty())
    {
        balls = con.getDetector(BALL)->getObjects();
    }
    std::vector<cv::Point> trajBalls;
    double avgRadius = 0;
    std::shared_ptr<Detector::BallObject> cueBall = nullptr;
    for(int i = 0; i < balls.size(); ++i)
    {
        auto realBall = std::dynamic_pointer_cast<Detector::BallObject>(balls[i]);
        if(realBall->ballType == Detector::BallType::CUE_BALL)
        {
            cueBall = realBall;
        } else {
            trajBalls.push_back(realBall->point);
        }
        
        avgRadius += realBall->radius;
    }

    if(cueBall != nullptr)
    {
        avgRadius /= trajBalls.size();
        trajBalls.insert(trajBalls.begin(), cueBall->point);
        con.getTrajectoryCalc()->setBalls(trajBalls);
        con.getTrajectoryCalc()->setBallRadius(avgRadius);
    }
}

void Detecting::onExit(Controller& con)
{

}

bool Detecting::handleEvent(Controller& con, const EventContainer& ev)
{
    return true;
}

Waiting::Waiting()
{

}

Waiting::~Waiting()
{

}

void Waiting::onEntry(Controller& con)
{

}

void Waiting::onDo(Controller& con)
{

}

void Waiting::onExit(Controller& con)
{

}

bool Waiting::handleEvent(Controller& con, const EventContainer& ev)
{
    return true;
}

