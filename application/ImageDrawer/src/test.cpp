#include <include/CairoDrawer.hpp>
#include <thread>

using namespace std::chrono_literals;

int main(int argc, char const *argv[])
{
    ImageDrawer::CairoDrawer drawer("/dev/fb0", "/dev/tty1", CAIRO_FORMAT_RGB16_565);

    drawer.setBackground(ImageDrawer::ColorRGBInt(0, 0, 0));

    drawer.setDrawColor(ImageDrawer::ColorRGBInt(255, 255, 255));

    drawer.drawLine(cv::Point(200, 200), cv::Point(400, 400));

    drawer.draw();

    std::this_thread::sleep_for(10s);

    return 0;
}
