#ifndef CAMERACONFIG_HPP
#define CAMERACONFIG_HPP

#include <fstream>
#include <string>

namespace ImageCapture
{
    class CameraConfig
    {
    public:
        CameraConfig(const std::string& filepath);
        ~CameraConfig();
        std::string getValue(const std::string& key);
    private:
        std::ifstream file;
    }; //CameraConfig
} // namespace ImageCapture

#endif //CAMERACONFIG_HPP
