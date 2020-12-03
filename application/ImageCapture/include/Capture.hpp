#ifndef CAPTURE_HPP
#define CAPTURE_HPP

#include "ICapture.hpp"
#include "CameraConfig.hpp"
#include <opencv2/videoio.hpp>
#include <map>
#ifdef __linux__
#include <linux/v4l2-controls.h>
#endif

namespace ImageCapture
{
#ifdef __linux__
    struct V4L2Settings
    {
        std::string name;
        int id;
        std::map<std::string, int> menu_items;
    };
#endif
    class Capture : public ICapture
    {
    public:
        /**
         * @brief Construct a new Capture object
         * 
         * @param device the device id
         */
        Capture(int device);
        virtual ~Capture();
        /**
         * @brief set region of interest to given points, resulting transfrom will move the roi into an image the size of width and height.
         * 
         * @param roi Region of interest in camera
         * @param width Image width of the resulting transform
         * @param height Image height of the resulting transform
         */
        virtual void setROI(std::array<cv::Point2f, 4>& roi, cv::Point2f::value_type width, cv::Point2f::value_type height);
        /**
         * @brief Get the newest frame (transformed if setROI)
         * 
         * @return cv::Mat 
         */
        virtual cv::Mat getFrame();
        /**
         * @brief Stop getter frame getting thread
         * 
         */
        virtual void stop();
        
    private:
    /**
         * @brief Update loop called by internal thread
         * 
         */
        virtual void update();
        CameraConfig config;
        cv::VideoCapture cap;
        std::array<cv::Point2f, 4> targetROI;
#ifdef __linux__
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
#endif
    }; //Capture
} // namespace ImageCapture

#endif //CAPTURE_HPP
