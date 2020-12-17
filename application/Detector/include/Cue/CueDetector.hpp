#ifndef CUEDETECTOR_HPP
#define CUEDETECTOR_HPP

#include <include/IDetector.hpp>
#include <include/IImageProcessing.hpp>

namespace Detector
{
    class CueDetector : public IDetector
    {
    public:
        /**
		 * @brief Construct a new Boundary Detector object
		 * 
		 * @param cap camera capture to use
		 */
        CueDetector(std::shared_ptr<Acquisition> cap);
        
        virtual ~CueDetector();
        /**
		 * @brief Get the boundary object
		 * 
		 * @return std::vector<std::shared_ptr<Object>> 
		 */
        std::vector<std::shared_ptr<Object>> getObjects();
    private:
        cv::Mat img;
    };
} //namespace Detector
#endif