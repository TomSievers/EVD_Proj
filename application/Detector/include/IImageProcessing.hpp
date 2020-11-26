#ifndef IIMAGEPROCESSING_HPP
#define IIMAGEPROCESSING_HPP

#include <memory>
#include <opencv2/core/mat.hpp>

namespace Detector
{
	class IImageProcessing
	{
	public:
		IImageProcessing(){}
		virtual ~IImageProcessing(){}
		virtual std::shared_ptr<void> process(cv::Mat& img) = 0;
	private:
		//data
	}; //IImageProcessing

} // namespace Detector


#endif //IIMAGEPROCESSING_HPP
