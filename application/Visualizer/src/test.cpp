#include <include/ObjectVisualizer.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

int main(int argc, char const *argv[])
{
    #if defined(__linux__) && defined(HAVE_CAIRO)
    Visualizer::ObjectVisualizer vis(CAIRO_FORMAT_ARGB32, cv::Point(0, 0), cv::Point(1000, 500));
    #else
    Visualizer::ObjectVisualizer vis(cv::Point(0, 0), cv::Point(1000, 500));
    #endif
    

    auto end = std::chrono::high_resolution_clock::now() + std::chrono::seconds(10);
    auto now = std::chrono::high_resolution_clock::now();

    while(std::chrono::duration_cast<std::chrono::milliseconds>(end-now).count() > 0)
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<cv::Point> trajectory;
        trajectory.push_back(cv::Point(1, 1));
        trajectory.push_back(cv::Point(999, 499));
        trajectory.push_back(cv::Point(500, 499));
        Visualizer::CueBall ball(cv::Point(250, 250));

        vis.update(trajectory, ball);
        
        now = std::chrono::high_resolution_clock::now();
    }
    

    return 0;
}