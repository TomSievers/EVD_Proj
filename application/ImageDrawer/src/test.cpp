#include <include/CairoDrawer.hpp>
#include <thread>

using namespace std::chrono_literals;

int main(int argc, char const *argv[])
{
    ImageDrawer::CairoDrawer drawer("/dev/fb0", "/dev/tty1", CAIRO_FORMAT_RGB16_565);

    drawer.setBackground(ImageDrawer::ColorRGBInt(0, 0, 0));

    drawer.setDrawColor(ImageDrawer::ColorRGBInt(255, 255, 255));

    drawer.drawLine(cv::Point(0, 0), cv::Point(200, 200));

    drawer.draw();

    drawer.setLineWidth(4);

    drawer.drawLine(cv::Point(200, 200), cv::Point(400, 400));

    drawer.setDrawColor(ImageDrawer::ColorRGBInt(255, 0, 0));

    drawer.drawCircle(cv::Point(500, 500), 100);

    drawer.draw();

    std::this_thread::sleep_for(5s);

    drawer.setBackground(ImageDrawer::ColorRGBInt(0, 125, 125));

    std::this_thread::sleep_for(2s);

    return 0;
}
