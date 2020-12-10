#include <include/TrajectoryCalculator.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, char const *argv[])
{
    cv::Mat img = cv::Mat::zeros(cv::Size(400, 200), CV_8UC3);
    int width = img.cols; int height = img.rows;

    std::array<cv::Point, 4> corners = {cv::Point(0,0), cv::Point(width,0), cv::Point(width, height), cv::Point(0, height)};
    std::vector<cv::Point> balls = {cv::Point(324, 133), cv::Point(250,50), cv::Point(300,150)};
    std::array<cv::Point, 2> cue = {cv::Point(355,42), cv::Point(337,102)};
    uint16_t radius = 8;
    
    TrajectoryCalculator::TrajectoryCalculator calc;
    calc.setBallRadius(8);
    calc.setPocketRadius(10);
    calc.setBalls(balls);
    calc.setTableCorners(corners);
    calc.setCue(cue);

    cv::line(img, cue[0], cue[1], cv::Scalar(255,0,255));

    for(uint8_t i = 0; i < balls.size(); i++){
        if(i == 0){
            cv::circle(img, balls[i], radius, cv::Scalar(255, 255, 255), -1);
        }else{
            cv::circle(img, balls[i], radius, cv::Scalar(255, 0, 0), -1);
        }
    }

    std::vector<cv::Point> trajectory = calc.getTrajectory();
    if (trajectory.size() > 1)
    {
        for (uint8_t i = 1; i < trajectory.size(); i++)
        {
            cv::line(img, trajectory[i-1], trajectory[i], cv::Scalar(0,0,255));
        }
    }
    

    cv::imshow("img", img);
    cv::waitKey(0);

    return 0;
}