#include <include/CairoDrawer.hpp>
#include <include/DebugDrawer.hpp>
#include <include/VsyncCairoDrawer.hpp>
#include <thread>
#include <chrono>
#define TEST

using namespace std::chrono_literals;

int main(int argc, char const *argv[])
{
#if defined(__linux__) && defined(HAVE_CAIRO) && !defined(HAVE_LIBDRM)
    ImageDrawer::CairoDrawer drawer("/dev/fb0", "/dev/tty1", CAIRO_FORMAT_RGB16_565);

    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now() + std::chrono::seconds(5);
    while(std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() >= 0)
    {
        start = std::chrono::high_resolution_clock::now();

        drawer.setBackground(ImageDrawer::ColorRGBInt(0, 0, 0));

        drawer.setDrawColor(ImageDrawer::ColorRGBInt(255, 255, 255));

        drawer.drawLine(cv::Point(0, 0), cv::Point(200, 200));

        drawer.draw();

        drawer.setLineWidth(4);

        drawer.drawLine(cv::Point(200, 200), cv::Point(1900, 1000));

        drawer.setDrawColor(ImageDrawer::ColorRGBInt(255, 0, 0));

        drawer.drawCircle(cv::Point(500, 500), 100);

        drawer.draw();


        auto now = std::chrono::high_resolution_clock::now();

        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(now-start).count() << std::endl;        
    }
#elif defined(__linux__) && defined(HAVE_CAIRO) && defined(HAVE_LIBDRM) 
    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now() + std::chrono::seconds(10);
    
    ImageDrawer::VsyncCairoDrawer drawer("/dev/dri/card1", "/dev/tty1");

    drawer.setBackground(ImageDrawer::ColorRGBInt(0, 0, 0));
    cv::Point startPos(100, 100);
    while(std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() >= 0)
    {
        auto test = std::chrono::high_resolution_clock::now();

        drawer.waitSwap();

        drawer.setBackground(ImageDrawer::ColorRGBInt(0, 0, 0));

        drawer.setDrawColor(ImageDrawer::ColorRGBInt(255, 0, 0));

        drawer.drawLine(cv::Point(0, 0), cv::Point(drawer.getScreenWidth(), drawer.getScreenHeight()));

        drawer.drawCircle(startPos, 100);

        drawer.draw();

        start = std::chrono::high_resolution_clock::now();

        drawer.swapDrawReady();

        startPos.x+=10;

        auto now = std::chrono::high_resolution_clock::now();

        std::cout << "total " << std::chrono::duration_cast<std::chrono::milliseconds>(now-test).count() << std::endl;
    }

#elif defined(DEBUGDRAWER)
    
    ImageDrawer::DebugDrawer drawer(1920, 1080);
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
