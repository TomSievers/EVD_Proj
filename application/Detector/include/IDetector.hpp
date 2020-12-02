#ifndef IDETECTOR_HPP
#define IDETECTOR_HPP

#include <memory>
#include <map>
#include <include/Acquisition.hpp>

namespace Detector
{
	struct Object
	{
		/* data */
	};

	struct Boundary : public Object
	{
		std::array<cv::Point, 4> corners;
		std::array<cv::Point, 6> pocketsLoc;
		double pocketRad;
	};

	enum VisionStep
	{
		ACQUISITION,
		EHANCEMENT,
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
