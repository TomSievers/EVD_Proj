#include "include/Configure/Configuration.hpp"

Configuration::Configuration()
{

}

Configuration::~Configuration()
{

}

Configuration& Configuration::getInstance()
{
    static Configuration config;
    return config;
}

void Configuration::setConfig(std::shared_ptr<Detector::Config> aConfig)
{
    config = aConfig;
}

std::shared_ptr<Detector::Config> Configuration::getConfig()
{
    return config;
}