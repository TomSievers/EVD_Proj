#ifndef ITRAJECTORY_HPP
#define ITRAJECTORY_HPP

#include <opencv2/core.hpp>
#include <array>

namespace TrajectoryCalculator
{
    class ITrajectory
    {
    public:
        /**
         * @brief Set the corners of the table to the given points.
         * 
         * @param corners Points representing the corners of the table.
         */
        virtual void setTableCorners(std::array<cv::Point, 4>& corners) = 0;

        /**
         * @brief Set the locations of the balls on the table.
         * 
         * @param balls Locations of the balls. First ball represents the cue ball.
         */ 
        virtual void setBalls(std::vector<cv::Point>& balls) = 0;

         /**
         * @brief Set the locations of the cue on the table.
         * 
         * @param cue 2 points of the cue needed to calculate the angle.
         */ 
        virtual void setCue(std::array<cv::Point, 2>& cue) = 0;

        /**
         * @brief Set the radius of the pocket.
         * 
         * @param radius Distance representing the ball radius.
         */ 
        virtual void setPocketRadius(uint16_t radius) = 0;

         /**
         * @brief Set the radius of the balls.
         * 
         * @param radius Distance representing the ball radius.
         */ 
        virtual void setBallRadius(uint16_t radius) = 0;

         /**
         * @brief Get the trajectory based on the set variables.
         * 
         * @return A list of points of which the trajectory is build out of.
         */ 
        virtual std::vector<cv::Point> getTrajectory() = 0;
        
    protected:
        ITrajectory(){};
        virtual ~ITrajectory(){};
        std::array<cv::Point, 4> tableCorners;
        std::vector<cv::Point> ballLocations;
        uint16_t ballRadius;
        uint16_t pocketRadius;
        std::array<cv::Point, 2> cuePoints;
    }; //ITrajectory

} // namespace TrajectoryCalculator


#endif //ITRAJECTORY_HPP
