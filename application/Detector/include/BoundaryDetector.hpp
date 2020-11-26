#ifndef BOUNDARYDETECTOR_HPP
#define BOUNDARYDETECTOR_HPP

#include <include/IDetector.hpp>
#include <include/IImageProcessing.hpp>
#include <array>

namespace Detector
{	
	class BoundaryDetector : public IDetector
	{
	public:
		/**
		 * @brief Construct a new Boundary Detector object
		 * 
		 * @param cap camera capture to use
		 */
		BoundaryDetector(std::shared_ptr<Acquisition> cap);
		virtual ~BoundaryDetector();
		/**
		 * @brief Get the boundary object
		 * 
		 * @return std::vector<std::shared_ptr<Object>> 
		 */
		virtual std::vector<std::shared_ptr<Object>> getObjects();
	private:
		cv::Mat img;
	}; //BoundaryDetector

} // namespace Detector


#endif //BOUNDARYDETECTOR_HPP
