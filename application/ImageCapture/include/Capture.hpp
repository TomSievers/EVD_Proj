#ifndef CAPTURE_HPP
#define CAPTURE_HPP

#include "ICapture.hpp"
#include "CameraConfig.hpp"
#include <opencv2/videoio.hpp>
#include <map>
#include <linux/v4l2-controls.h>

namespace ImageCapture
{

    struct V4L2Settings
    {
        std::string name;
        int id;
        std::map<std::string, int> menu_items;
    };

    class Capture : public ICapture
    {
    public:
        Capture(int device);
        Capture(const std::string& device);
        virtual ~Capture();
        virtual void setROI(std::array<cv::Point2f, 4>& roi, cv::Point2f::value_type width, cv::Point2f::value_type height);
        virtual cv::Mat getFrame();
        virtual void stop();
        virtual void update();
    private:
        CameraConfig config;
        cv::VideoCapture cap;
        std::array<cv::Point2f, 4> targetROI = {cv::Point2f(0, 0), cv::Point2f(200, 0), cv::Point2f(200, 100), cv::Point2f(0, 100)};
        V4L2Settings settings[7] = {
            {
                "auto_exposure",
                V4L2_CID_EXPOSURE_AUTO,
                {
                    {"auto", V4L2_EXPOSURE_AUTO},
                    {"manual", V4L2_EXPOSURE_MANUAL}
                }
            }, 
            {
                "iso_sensitivity_auto",
                V4L2_CID_ISO_SENSITIVITY_AUTO,
                {
                    {"auto", V4L2_ISO_SENSITIVITY_AUTO},
                    {"manual", V4L2_ISO_SENSITIVITY_MANUAL}
                }
            }, 
            {
                "exposure_time_absolute",
                V4L2_CID_EXPOSURE_ABSOLUTE,
                {}
            }, 
            {
                "iso_sensitivity",
                V4L2_CID_ISO_SENSITIVITY,
                {}
            }, 
            {
                "white_balance_auto_preset",
                V4L2_CID_AUTO_N_PRESET_WHITE_BALANCE,
                {
                    {"auto", V4L2_WHITE_BALANCE_AUTO},
                    {"manual", V4L2_WHITE_BALANCE_MANUAL},
                    {"incandescent", V4L2_WHITE_BALANCE_INCANDESCENT},
                    {"fluorescent", V4L2_WHITE_BALANCE_FLUORESCENT},
                    {"fluorescent_h", V4L2_WHITE_BALANCE_FLUORESCENT_H},
                    {"horizon", V4L2_WHITE_BALANCE_HORIZON},
                    {"daylight", V4L2_WHITE_BALANCE_DAYLIGHT},
                    {"flash", V4L2_WHITE_BALANCE_FLASH},
                    {"cloudy", V4L2_WHITE_BALANCE_CLOUDY},
                    {"shade", V4L2_WHITE_BALANCE_SHADE}
                }
            },
            {
                "red_balance",
                V4L2_CID_RED_BALANCE,
                {}
            }, 
            {
                "blue_balance",
                V4L2_CID_BLUE_BALANCE,
                {}
            }
        };
    }; //Capture
} // namespace ImageCapture

#endif //CAPTURE_HPP
