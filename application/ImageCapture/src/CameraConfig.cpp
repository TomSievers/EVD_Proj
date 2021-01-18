#include <include/CameraConfig.hpp>
#include <iostream>
#include <regex>

#define LINE_MAX 500

namespace ImageCapture
{
    const std::string WHITESPACE = " \n\r\t\f\v";

    std::string ltrim(const std::string& s)
    {
        size_t start = s.find_first_not_of(WHITESPACE);
        return (start == std::string::npos) ? "" : s.substr(start);
    }

    std::string rtrim(const std::string& s)
    {
        size_t end = s.find_last_not_of(WHITESPACE);
        return (end == std::string::npos) ? "" : s.substr(0, end + 1);
    }

    std::string trim(const std::string& s)
    {
        return rtrim(ltrim(s));
    }

    CameraConfig::CameraConfig(const std::string& filepath) : file(filepath)
    {
        if(!file.is_open())
        {
            std::string func = __PRETTY_FUNCTION__;
            throw std::runtime_error(func + ": File not found: " + filepath);
        }
    }

    CameraConfig::~CameraConfig()
    {
        if(file.is_open())
        {
            file.close();
        }
    }

    std::string CameraConfig::getValue(const std::string& key)
    {
        std::regex reg("[^:]+");
        char line[LINE_MAX] = "";
        std::string value;
        file.clear();
        file.seekg(0);
        
        while(file.getline(line, LINE_MAX, '\n'))
        {
            std::string sline = line;
            std::sregex_iterator begin(sline.begin(), sline.end(), reg);
            std::sregex_iterator end;

            if(begin != end)
            {
                std::string fkey = trim(begin->str());
                if(fkey == key)
                {
                    ++begin;
                    if(begin != end)
                    {
                        value = trim(begin->str());
                        file.sync();
                        return value;
                    }
                }
            }
        }
        value.clear();
        file.sync();
        return value;
    }
} // namespace ImageCapture
