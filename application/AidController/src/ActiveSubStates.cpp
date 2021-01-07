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
    for(std::size_t i = 0; i < balls.size(); ++i)
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
        avgRadius /= static_cast<double>(trajBalls.size());
        trajBalls.insert(trajBalls.begin(), cueBall->point);
        con.getTrajectoryCalc()->setBalls(trajBalls);
        con.getTrajectoryCalc()->setBallRadius(static_cast<uint16_t>(avgRadius));
        bool ballMoved = false;
        while(!ballMoved)
        {
            auto cue = con.getDetector(CUE)->getObjects();
            if(!cue.empty())
            {
                auto realCue = std::dynamic_pointer_cast<Detector::CueObject>(cue[0]);
                std::array<cv::Point, 2> cuePoints = {realCue->endPoints[1], realCue->endPoints[0]};
                con.getTrajectoryCalc()->setCue(cuePoints);
                auto traj = con.getTrajectoryCalc()->getTrajectory();
                Visualizer::CueBall ball(cueBall->point);
                ball.radius = avgRadius*2;
                con.getVisualizer()->update(traj, ball);
            }
        }
        con.scheduleEvent(Event::MOVING);
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

bool checkBallPosition(const cv::Point& a, const cv::Point& b)
{
    return (a.x <= b.x+2 && a.x >= b.x-2 && a.y <= b.y+2 && a.y >= b.y-2);
}

void Waiting::onDo(Controller& con)
{
    std::vector<std::shared_ptr<Detector::Object>> prevBalls;
    bool moving = true;
    while(moving)
    {
        auto balls = con.getDetector(BALL)->getObjects();
        moving = false;
        if(!prevBalls.empty() && !balls.empty())
        {
            for(auto prevBall : prevBalls)
            {
                bool changed = true;
                auto prevRealBall = std::dynamic_pointer_cast<Detector::BallObject>(prevBall);
                for(auto ball : balls)
                {
                    auto realBall = std::dynamic_pointer_cast<Detector::BallObject>(ball);
                    if(checkBallPosition(realBall->point, prevRealBall->point))
                    {
                        changed = false;
                    }
                }
                if(changed)
                {
                    moving = true;
                    break;
                }
            }
        }
        prevBalls = balls;
    }
    con.scheduleEvent(Event::STEADY);

}

void Waiting::onExit(Controller& con)
{

}

bool Waiting::handleEvent(Controller& con, const EventContainer& ev)
{
    return true;
}

