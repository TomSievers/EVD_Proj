#ifndef TRAJECTORYCALCULATOR_HPP
#define TRAJECTORYCALCULATOR_HPP

#include <opencv2/core.hpp>
#include "ITrajectory.hpp"

namespace TrajectoryCalculator
{
    struct Line
    {
        cv::Point pt1;
        cv::Point pt2;
    };

    class TrajectoryCalculator: ITrajectory
    {
    public:
        /**
         * @brief Construct a new Trajectory Calculator object
         * 
         */
        TrajectoryCalculator() {};
        ~TrajectoryCalculator() {};
    
        /**
         * @brief Set the corners of the table to the given points.
         * 
         * @param corners Points representing the corners of the table.
         */
        virtual void setTableCorners(std::array<cv::Point, 4>& corners);

        /**
         * @brief Set the locations of the balls on the table.
         * 
         * @param balls Locations of the balls. First ball represents the cue ball.
         */ 
        virtual void setBalls(std::vector<cv::Point>& balls);

         /**
         * @brief Set the locations of the cue on the table.
         * 
         * @param cue 2 points of the cue needed to calculate the angle.
         */ 
        virtual void setCue(std::array<cv::Point, 2>& cue);

        /**
         * @brief Set the radius of the pocket.
         * 
         * @param radius Distance representing the ball radius.
         */ 
        virtual void setPocketRadius(uint16_t radius);

         /**
         * @brief Set the radius of the balls.
         * 
         * @param radius Distance representing the ball radius.
         */ 
        virtual void setBallRadius(uint16_t radius);

         /**
         * @brief Get the trajectory based on the set variables.
         * 
         * @return A list of points of which the trajectory is build out of.
         */ 
        virtual std::vector<cv::Point> getTrajectory();

        /**
         * @brief Get the next point in the trajectory.
         * 
         * @param start Start point of the next line.
         * @param angle Angle of the next line.
         * @param hitBalls List of balls that have been hit.
         * @param flag Should the calculation continue;
         * @return Inverted angle in radians.
         */ 
        virtual cv::Point nextPoint(cv::Point *start, float *angle, std::vector<cv::Point> *hitBalls, uint8_t *flag);
    private:
         /**
         * @brief Calculate the angle between 2 points.
         * 
         * @param pt1 First point.
         * @param pt2 Second point.
         * @return Calculated angle in Radians.
         */ 
        virtual float ptToAngle(cv::Point& pt1, cv::Point& pt2);
         /**
         * @brief Predict a future point based on start and angle.
         * 
         * @param pt Starting point.
         * @param angle Angle in radians.
         * @param distance Distance of the starting point to the new point.
         * @return Predicted point
         */ 
        virtual cv::Point predictPoint(cv::Point& pt, float& angle, uint32_t distance);
         /**
         * @brief Calculate at which point the lines intersect.
         * 
         * @param line1 First line.
         * @param line1 Second line.
         * @param inter Point of intersection
         * @return Point of intersection.
         */ 
        uint8_t lineIntersection(Line line1, Line line2, cv::Point *inter);

         /**
         * @brief Calculate at which point the circle intersects with the line.
         * 
         * @param line Line to check for intersection.
         * @param circle Center point of the circle.
         * @param circleRadius Radius of the circle.
         * @return Point of intersection.
         */ 
        virtual std::vector<cv::Point> lineCircleIntersection(Line& line, cv::Point& circle, uint16_t circleRadius);
        /**
         * @brief Calculate the distance from a point to a line.
         * 
         * @param pt1 First point of line to check for distance.
         * @param pt2 Second point of line to check for distance.
         * @param pt Point to compare to.
         * @return Distance to the line.
         */ 
        virtual uint32_t linePointDistance(cv::Point& pt1, cv::Point& pt2, cv::Point& pt);
    }; //TrajectoryCalculator
} // namespace TrajectoryCalculator

#endif //TRAJECTORYCALCULATOR_HPP