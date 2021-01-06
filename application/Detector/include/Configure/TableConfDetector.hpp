#ifndef TABLE_CONF_DETECTOR_HPP
#define TABLE_CONF_DETECTOR_HPP

#include "include/IImageProcessing.hpp"
#include "include/IDetector.hpp"
#include "include/Acquisition.hpp"

namespace Detector
{
    class TableConfDetector : public IDetector
    {
        public:
            TableConfDetector(std::shared_ptr<Acquisition> cap);
            virtual ~TableConfDetector();

            /**
             * @brief This function returns the found average min and max pixel value for the table
             * It will run over several images to determine the average
             * 
             * @return std::vector<std::shared_ptr<Object>> A struct which contains the min and max hsv values for the table
             */
            std::vector<std::shared_ptr<Object>> getObjects();
    };
}

#endif