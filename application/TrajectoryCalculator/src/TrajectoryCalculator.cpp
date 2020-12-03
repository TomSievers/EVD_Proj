#include <include/TrajectoryCalculator.hpp>
#include <numeric>
#include <iostream>

#define MAX_COLLISIONS 5

namespace TrajectoryCalculator
{

    void TrajectoryCalculator::setTableCorners(std::array<cv::Point, 4>& corners)
    {
        tableCorners = corners;
    }

    void TrajectoryCalculator::setBalls(std::vector<cv::Point>& balls)
    {
        ballLocations = balls;
    }

    void TrajectoryCalculator::setCue(std::array<cv::Point, 2>& cue)
    {
        cuePoints = cue;
    }

    void TrajectoryCalculator::setBallRadius(uint16_t radius)
    {
        ballRadius = radius;
    }

    void TrajectoryCalculator::setPocketRadius(uint16_t radius)
    {
        pocketRadius = radius;
    }

    std::vector<cv::Point> TrajectoryCalculator::getTrajectory()
    {
        std::vector<cv::Point> trajectory = {};

        if (ballLocations.size() > 0)
        {
            float angle = ptToAngle(cuePoints[0], cuePoints[1]);
            cv::Point fp = predictPoint(cuePoints[1], angle, tableCorners[2].x);
            if (linePointDistance(cuePoints[0], fp, ballLocations[0]) <= ballRadius)
            {
                uint8_t collisions = 0;

                cv::Point start = ballLocations[0];
                trajectory.push_back(start);

                std::vector<cv::Point> hitBalls = {};
                uint8_t flag = 1;

                while (collisions < MAX_COLLISIONS && flag == 1)
                {
                    trajectory.push_back(nextPoint(&start, &angle, &hitBalls, &flag));
                    ++collisions;
                }
            }
        }
        
        return trajectory;
    }

    cv::Point TrajectoryCalculator::nextPoint(cv::Point *start, float *angle, std::vector<cv::Point> *hitBalls, uint8_t *flag)
    {
        Line trjLine;
        trjLine.pt1 = *start;
        trjLine.pt2 = predictPoint(*start, *angle, tableCorners[2].x+tableCorners[2].y);

        std::vector<cv::Point> inPlayBalls = {};
        std::vector<uint32_t> distances = {};
        std::vector<std::vector<cv::Point>::size_type> ballOrder = {};
        for (uint8_t i = 1; i < ballLocations.size(); i++)
        {
            if (std::find((*hitBalls).begin(), (*hitBalls).end(), ballLocations[i]) == (*hitBalls).end())
            {
                inPlayBalls.push_back(ballLocations[i]);
                distances.push_back(euclideanDistance(*start, ballLocations[i]));
            }
        }

        std::vector<size_t> ordered(inPlayBalls.size());
        std::iota(ordered.begin(), ordered.end(), 0);
        stable_sort(ordered.begin(), ordered.end(),[&distances](size_t i1, size_t i2) {return distances[i1] < distances[i2];});
        

        for(size_t i: ordered){
            cv::Point ball = inPlayBalls[i];
            if (linePointDistance(trjLine.pt1, trjLine.pt2, ball) <= ballRadius * 2)
            {
                std::vector<cv::Point> intersections = lineCircleIntersection(trjLine, ball, (uint16_t)(ballRadius*2)); 
                if (intersections.size() <= 0 || intersections[0] == *start)
                {
                    continue;
                }

                float nextAngle = invertAngle(ptToAngle(ball, intersections[0]));
                if (*angle > ptToAngle(*start, ball))
                {
                    *angle = nextAngle + (float)(M_PI / 2);
                }else
                {
                     *angle = nextAngle - (float)(M_PI / 2);
                }
                *start = intersections[0];
                return intersections[0];
            }
            
        }

        for (uint8_t i = 0; i < tableCorners.size(); i++)
        {
            if (linePointDistance(trjLine.pt1, trjLine.pt2, tableCorners[i]) <= pocketRadius)
            {
                std::vector<cv::Point> intersections = lineCircleIntersection(trjLine, tableCorners[i], pocketRadius); 
                if (intersections.size() <= 0 || intersections[0] == *start)
                {
                    continue;
                }
                flag = 0;
                return intersections[0];
            }
            
        }
        

        std::array<uint8_t, 2> sides = {0,1};
        if (*angle >= 0)
        {
            sides[0] = 2;
        }

        if (abs(*angle) >= (M_PI / 2))
        {
            sides[1] = 3;
        }
        
        for (uint8_t i = 0; i < 2; i++)
        {
            Line boundary;
            uint8_t side = sides[i];
            boundary.pt1 = tableCorners[side];
            if(side == 3){
                boundary.pt2 = tableCorners[0];
            }else{
                boundary.pt2 = tableCorners[side + 1];
            }

            cv::Point inter;
            char found = lineIntersection(trjLine, boundary, &inter);
            
            if(!found){
                continue;
            }
            
            if (side == 1 || side == 3)
            {
                if (*angle > 0)
                {
                   *angle = (float)(M_PI - *angle);
                }else
                {
                    *angle = (float)(-(*angle + M_PI));
                }
            }else{
                *angle = -*angle;
            }

            if (*angle > M_PI)
            {
                *angle = (float)(M_PI - *angle);
            }else if (*angle < -M_PI)
            {
                *angle = (float)(M_PI + *angle);
            }
            
            *start = inter;
            return *start;
        }
        
        flag = 0;
        return trjLine.pt2;
    }

    inline float TrajectoryCalculator::invertAngle(float angle)
    {
        return fmodf(angle + (float)M_PI, 2 * (float)M_PI);
    }

    inline float TrajectoryCalculator::ptToAngle(cv::Point& pt1, cv::Point& pt2)
    {
        return atan2f((float)(pt2.y - pt1.y), (float)(pt2.x - pt1.x));
    }
         
    inline uint32_t TrajectoryCalculator::euclideanDistance(cv::Point& pt1, cv::Point& pt2)
    {
        return (int) sqrt(pow(pt2.x - pt1.x, 2) + pow(pt2.y - pt1.y, 2));
    }
         
    inline cv::Point TrajectoryCalculator::predictPoint(cv::Point& pt, float& angle, uint32_t distance)
    {
        return cv::Point(pt.x + (int)((float)distance * cosf(angle)), pt.y + (int)((float)distance * sinf(angle)));
    }

    uint8_t TrajectoryCalculator::lineIntersection(Line line1, Line line2, cv::Point *inter)
    {
        float s1_x = (float)(line1.pt2.x - line1.pt1.x); 
        float s1_y = (float)(line1.pt2.y - line1.pt1.y);
        float s2_x = (float)(line2.pt2.x - line2.pt1.x); 
        float s2_y = (float)(line2.pt2.y - line2.pt1.y);

        float s, t;
        s = (-s1_y * (float)(line1.pt1.x - line2.pt1.x) + s1_x * (float)(line1.pt1.y - line2.pt1.y)) / (-s2_x * s1_y + s1_x * s2_y);
        t = ( s2_x * (float)(line1.pt1.y - line2.pt1.y) - s2_y * (float)(line1.pt1.x - line2.pt1.x)) / (-s2_x * s1_y + s1_x * s2_y);

        if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
        {
            *inter = cv::Point((int)((float)line1.pt1.x + (t * s1_x)),  (int)((float)line1.pt1.y + (t * s1_y)));
            return 1;
        }

        return 0;
    }

    std::vector<cv::Point> TrajectoryCalculator::lineCircleIntersection(Line& line, cv::Point& circle, uint16_t circleRadius)
    {
        std::vector<cv::Point> intersections = {};

        uint32_t a = (uint32_t)(pow(line.pt1.x-line.pt2.x, 2) + pow(line.pt1.y-line.pt2.y, 2));
        int32_t b = ((line.pt1.x-line.pt2.x) * (circle.x - line.pt1.x)) + ((line.pt1.y-line.pt2.y) * (circle.y - line.pt1.y));
        uint32_t c = (uint32_t)(pow(circle.x - line.pt1.x, 2) + pow(circle.y - line.pt1.y, 2) - pow(circleRadius, 2));
        
        int32_t d = b * b - a * c;
        if(d < 0)
        {
            return intersections;
        }

        float s = sqrtf((float)d);
        float t1 = ((float)-b - s) / (float)a;
        float t2 = ((float)-b + s) / (float)a;

        if(0 <= t1 <= 1)
        {
            intersections.push_back(cv::Point((int)((1-t1) * (float)line.pt1.x + t1 * (float)line.pt2.x), (int)((1-t1) * (float)line.pt1.y + t1 * (float)line.pt2.y)));
        }

        if(0 <= t2 <= 1)
        {
            intersections.push_back(cv::Point((int)((1-t2) * (float)line.pt1.x + t2 * (float)line.pt2.x), (int)((1-t2) * (float)line.pt1.y + t2 * (float)line.pt2.y)));
        }

        return intersections;
    }
        
    uint32_t TrajectoryCalculator::linePointDistance(cv::Point& pt1, cv::Point& pt2,  cv::Point& pt)
    {
        int a = pt.x - pt1.x;
        int b = pt.y - pt1.y;
        int c = pt2.x - pt1.x;
        int d = pt2.y - pt1.y;

        int dot = a * c + b * d;
        int len = c * c + d * d;
        float f = -1;
        if (len != 0)
        {
            f = (float)dot/(float)len;
        }

        float xx; float yy;
        if (f < 0)
        {
            xx = (float)pt1.x;
            yy = (float)pt1.y;
        }else if (f < 0)
        {
            xx = (float)pt2.x;
            yy = (float)pt2.y;
        }else
        {
            xx = (float)pt1.x + f * (float)c;
            yy = (float)pt1.y + f * (float)d;
        }
        
        return (uint32_t)sqrtf(powf((float)pt.x - xx,2)+powf((float)pt.y - yy,2));
    }
} // namespace TrajectoryCalculator