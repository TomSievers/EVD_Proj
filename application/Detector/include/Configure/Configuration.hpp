#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include "include/IDetector.hpp"

class Configuration
{
    public:
        static Configuration& getInstance();
        
        void setConfig(std::shared_ptr<Detector::Config> aConfig);
        std::shared_ptr<Detector::Config> getConfig();

        Configuration(const Configuration& configuration) = delete;
        void operator=(const Configuration& configuration) = delete;
    private:
        Configuration();
        virtual ~Configuration();

        std::shared_ptr<Detector::Config> config;
};

#endif