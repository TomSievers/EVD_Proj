#include <include/ActiveSubStates.hpp>
#include <include/Controller.hpp>
#include <opencv2/core.hpp>
#include <Detector/include/Change/ChangeDetector.hpp>
#include <Detector/include/Cue/CueDetector.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>

Detecting::Detecting()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

Detecting::~Detecting()
{
    
}

void Detecting::onEntry(Controller& con)
{
    (void)con;
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
        con.getTrajectoryCalc()->setBallRadius(static_cast<uint16_t>(avgRadius*1.2));
        bool ballMoved = false;
        auto changeDet = std::dynamic_pointer_cast<Detector::ChangeDetector>(con.getDetector(CHANGE));
        std::array<cv::Point2i, 4> roi = {
            cv::Point(cueBall->point.x-(int)round(avgRadius), cueBall->point.y-(int)round(avgRadius)),
            cv::Point(cueBall->point.x+(int)round(avgRadius), cueBall->point.y-(int)round(avgRadius)),
            cv::Point(cueBall->point.x+(int)round(avgRadius), cueBall->point.y+(int)round(avgRadius)),
            cv::Point(cueBall->point.x-(int)round(avgRadius), cueBall->point.y+(int)round(avgRadius))
        };
        std::static_pointer_cast<Detector::CueDetector>(con.getDetector(CUE))->setBalls(trajBalls, avgRadius);
        changeDet->setRoi(roi);
        while(!ballMoved)
        {
            auto cue = con.getDetector(CUE)->getObjects();
            if(!cue.empty())
            {
                auto realCue = std::dynamic_pointer_cast<Detector::CueObject>(cue[0]);
                int frontIndex = -1;
                double dist = 1200;
                for(std::size_t i = 0; i < realCue->endPoints.size(); ++i)
                {
                    double dist2 = sqrt(pow((double)(realCue->endPoints[i].x - cueBall->point.x), 2) +  pow((double)(realCue->endPoints[i].y - cueBall->point.y), 2));
                    if(sqrt(pow((double)(realCue->endPoints[i].x - cueBall->point.x), 2) +  pow((double)(realCue->endPoints[i].y - cueBall->point.y), 2)) < dist)
                    {
                        frontIndex = i;
                        dist = dist2;
                    }
                }
                std::array<cv::Point, 2> cuePoints = {realCue->endPoints[(frontIndex+1)%realCue->endPoints.size()], realCue->endPoints[frontIndex]};
                con.getTrajectoryCalc()->setCue(cuePoints);
                auto traj = con.getTrajectoryCalc()->getTrajectory();
                Visualizer::CueBall ball(cueBall->point);
                ball.radius = avgRadius*2;
                con.getVisualizer()->update(traj, ball);
            } else {
                std::vector<cv::Point> traj;
                Visualizer::CueBall ball(cueBall->point);
                ball.radius = avgRadius*2;
                con.getVisualizer()->update(traj, ball);
            }

            auto change = changeDet->getObjects();
            if(!change.empty())
            {
                std::shared_ptr<Detector::ChangeObject> changeObject = std::static_pointer_cast<Detector::ChangeObject>(change.at(0));
                if(changeObject->moving)
                {
                    ballMoved = true;
                }
            }
            cv::waitKey(1);
        }
        std::vector<cv::Point> traj;
        con.getVisualizer()->update(traj, Visualizer::CueBall());
        changeDet->clearRoi();
        con.scheduleEvent(Event::MOVING);
    }
}

void Detecting::onExit(Controller& con)
{
    (void)con;
}

bool Detecting::handleEvent(Controller& con, const EventContainer& ev)
{
    (void)con;
    (void)ev;
    return true;
}

Waiting::Waiting()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    
}

Waiting::~Waiting()
{
    
}

void Waiting::onEntry(Controller& con)
{
    (void)con;
}

void Waiting::onDo(Controller& con)
{
    std::vector<std::shared_ptr<Detector::Object>> prevBalls;
    bool moving = true;
    uint32_t stopCount = 0;
    while(moving)
    {
        auto change = con.getDetector(CHANGE)->getObjects();
        if(!change.empty())
        {
            std::shared_ptr<Detector::ChangeObject> changeObject = std::static_pointer_cast<Detector::ChangeObject>(change.at(0));
            if(!changeObject->moving)
            {
                ++stopCount;
            } else {
                stopCount = 0;
            }

            if(stopCount >= 5)
            {
                moving = false;
            }
        }
    }
    con.scheduleEvent(Event::STEADY);

}

void Waiting::onExit(Controller& con)
{
    (void)con;
}

bool Waiting::handleEvent(Controller& con, const EventContainer& ev)
{
    (void)con;
    (void)ev;
    return true;
}

