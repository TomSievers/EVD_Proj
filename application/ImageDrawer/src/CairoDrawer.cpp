#include <include/CairoDrawer.hpp>

#include <fcntl.h> 
#include <sys/ioctl.h>
#include <sys/kd.h>
#include <unistd.h>
#include <errno.h>

namespace ImageDrawer
{
        CairoDrawer::CairoDrawer(const std::string& framebuffer, const std::string& terminal, cairo_format_t format)
        {

        }

        CairoDrawer::~CairoDrawer()
        {

        }
    
        void CairoDrawer::setDrawColor(const ColorRGBInt& color)
        {

        }
        void CairoDrawer::setBackground(const ColorRGBInt& color)
        {

        }
        void CairoDrawer::drawCircle(const cv::Point& center, double radius)
        {

        }
        void CairoDrawer::drawLine(const cv::Point& pointA, const cv::Point& pointB)
        {

        }
        void CairoDrawer::setLineWidth(int thickness)
        {

        }
        void CairoDrawer::draw()
        {

        }
        

        bool CairoDrawer::setTty(const std::string& device, TTY_MODE mode)
        {
            int fd = open(device.c_str(), O_RDWR);
            bool succes = true;

            if (fd < 0) {
                throw(std::runtime_error("Could not open terminal: " + device + " errno: " + std::to_string(errno)));
                succes = false;
            }

            if (mode)
            {
                if (ioctl( fd, KDSETMODE, KD_GRAPHICS))
                {
                    throw(std::runtime_error("Could not set terminal to KD_GRAPHICS mode: " + device + " errno: " + std::to_string(errno)));
                    succes = false;
                }
            } else {
                if (ioctl( fd, KDSETMODE, KD_TEXT))
                {
                    throw(std::runtime_error("Could not set terminal to KD_GRAPHICS mode: " + device + " errno: " + std::to_string(errno)));
                    succes = false;
                }
            }

            close(fd);
            return succes;
        }
    }; //CairoDrawer

} // namespace ImageDrawer