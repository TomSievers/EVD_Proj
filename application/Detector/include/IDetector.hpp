#ifndef IDETECTOR_HPP
#define IDETECTOR_HPP

#include <memory>
#include <map>
#include <include/Acquisition.hpp>

namespace Detector
{
    struct Line
    {
        double a, b, c;
		Line(){};
        Line(double a, double b, double c) :
               a(a), b(b), c(c)
        {};
        ~Line(){};
    };
	struct Object
	{
		Object() {};
		virtual ~Object() {}
	};

	struct Boundary : public Object
	{
		std::array<cv::Point, 4> corners;
		std::array<cv::Point, 6> pocketsLoc;
		double pocketRad;
	};

	struct CueObject : public Object
	{
		cv::Point center;
		std::vector<cv::Point> endPoints;
		Line line;
	};
	enum BallType
    {
		NOT_CUE_BALL,
        CUE_BALL
    };

    struct BallObject : public Object
    {
        uint8_t percentageWhite;
		std::vector<cv::Point> ballContourPoints;
		std::vector<cv::Point> whiteContourPoints;
		cv::Point point;
        float radius;
        BallType ballType;
    };

	enum VisionStep
	{
		ACQUISITION,
		ENHANCEMENT,
		SEGMENTATION,
		FEATURE_EXTRACT,
		CLASSIFICATION
	};
	
	class IDetector
	{
	public:
		/**
		 * @brief Construct a new IDetector object
		 * 
		 * @param cap the image capture to use
		 */
		IDetector(std::shared_ptr<Acquisition> cap)
		{
			processors[ACQUISITION] = cap;
		}
		virtual ~IDetector(){}
		/**
		 * @brief Get the list of objects from the detector
		 * 
		 * @return std::vector<std::shared_ptr<Object>> 
		 */
		virtual std::vector<std::shared_ptr<Object>> getObjects() = 0;
	protected:
		std::map<VisionStep, std::shared_ptr<IImageProcessing>> processors;
		//data
	}; //IDetector

} // namespace Detector


#endif //IDETECTOR_HPP
