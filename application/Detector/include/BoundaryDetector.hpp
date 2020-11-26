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
		BoundaryDetector();
		virtual ~BoundaryDetector();
		virtual std::vector<std::shared_ptr<Object>> getObjects();
	private:
		cv::Mat img;
		std::vector<std::shared_ptr<IImageProcessing>> processors;
	}; //BoundaryDetector

} // namespace Detector


#endif //BOUNDARYDETECTOR_HPP
