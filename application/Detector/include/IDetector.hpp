#ifndef IDETECTOR_HPP
#define IDETECTOR_HPP

#include <memory>
#include <vector>

namespace Detector
{
	struct Object
	{
		/* data */
	};
	
	class IDetector
	{
	public:
		IDetector(){}
		virtual ~IDetector(){}
		virtual std::vector<std::shared_ptr<Object>> getObjects() = 0;
	private:
		//data
	}; //IDetector

} // namespace Detector


#endif //IDETECTOR_HPP
