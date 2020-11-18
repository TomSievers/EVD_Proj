#ifndef CAMERACONFIG_HPP
#define CAMERACONFIG_HPP

#include <fstream>
#include <string>

namespace ImageCapture
{
    class CameraConfig
    {
    public:
        /**
         * @brief Construct a new Camera Config object
         * 
         * @param filepath path to the config file
         */
        CameraConfig(const std::string& filepath);
        ~CameraConfig();
        /**
         * @brief Get the value of the key in the config file
         * 
         * @param key Key to search for in the config file
         * @return std::string 
         */
        std::string getValue(const std::string& key);
    private:
        std::ifstream file;
    }; //CameraConfig
} // namespace ImageCapture

#endif //CAMERACONFIG_HPP
