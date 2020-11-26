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
		/**
		 * @brief process provided image
		 * 
		 * @param img image to be processed
		 * @return std::shared_ptr<void> pointer to anything that possibly needs to be returned
		 */
		virtual std::shared_ptr<void> process(cv::Mat& img) = 0;
	private:
		//data
	}; //IImageProcessing

} // namespace Detector


#endif //IIMAGEPROCESSING_HPP
