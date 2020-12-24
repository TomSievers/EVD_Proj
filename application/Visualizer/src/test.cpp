#include <include/ObjectVisualizer.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

int main(int argc, char const *argv[])
{
    Visualizer::ObjectVisualizer vis(cv::Point(0, 0), cv::Point(1000, 500));

    std::vector<cv::Point> trajectory;
    trajectory.push_back(cv::Point(1, 1));
    trajectory.push_back(cv::Point(999, 499));
    trajectory.push_back(cv::Point(500, 499));
    Visualizer::CueBall ball(cv::Point(250, 250));

    vis.update(trajectory, ball);

    std::this_thread::sleep_for(5s);

    trajectory.clear();
    trajectory.push_back(cv::Point(0, 0));
    trajectory.push_back(cv::Point(0, 250));
    trajectory.push_back(cv::Point(1000, 500));
    Visualizer::CueBall ball2(cv::Point(500, 250));

    vis.update(trajectory, ball2);

    std::this_thread::sleep_for(5s);

    return 0;
}