#include <include/CairoDrawer.hpp>
#include <thread>

using namespace std::chrono_literals;

int main(int argc, char const *argv[])
{
#if defined(__linux__) && defined(HAVE_CAIRO) && !defined(DEBUG)
    ImageDrawer::CairoDrawer drawer("/dev/fb0", "/dev/tty1", CAIRO_FORMAT_RGB16_565);

    drawer.setBackground(ImageDrawer::ColorRGBInt(0, 0, 0));

    drawer.setDrawColor(ImageDrawer::ColorRGBInt(255, 255, 255));

    drawer.drawLine(cv::Point(0, 0), cv::Point(200, 200));

    drawer.draw();

    drawer.setLineWidth(4);

    drawer.drawLine(cv::Point(200, 200), cv::Point(1900, 1000));

    drawer.setDrawColor(ImageDrawer::ColorRGBInt(255, 0, 0));

    drawer.drawCircle(cv::Point(500, 500), 100);

    drawer.draw();

    std::this_thread::sleep_for(2s);

    drawer.setBackground(ImageDrawer::ColorRGBInt(0, 125, 125));

    drawer.setDrawColor(ImageDrawer::ColorRGBInt(255, 0, 0));
    drawer.drawCircle(cv::Point(500, 1000), 100);

    drawer.drawLine(cv::Point(500, 1000), cv::Point(500, 1200));

    drawer.draw();

    std::this_thread::sleep_for(2s);

#endif

    return 0;
}
