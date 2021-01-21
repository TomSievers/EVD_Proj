#ifndef CUEDETECTOR_HPP
#define CUEDETECTOR_HPP

#include <include/IDetector.hpp>
#include <include/IImageProcessing.hpp>

namespace Detector
{
    struct Balls
    {
        std::vector<cv::Point> pos;
        double radius;
        Balls() : radius(0){}
        Balls(std::vector<cv::Point> pos, double radius) : pos(pos), radius(radius){}
        ~Balls(){};
    };

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
        void setBalls(std::vector<cv::Point> balls, double radius);
    private:
        Balls balls;
        cv::Mat img;
    };
} //namespace Detector
#endif