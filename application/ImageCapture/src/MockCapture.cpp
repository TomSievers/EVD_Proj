#include <include/MockCapture.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace ImageCapture
{
    MockCapture::MockCapture(const std::string& imgPath) : img(cv::imread(imgPath))
    {
        if(img.empty())
        {
            std::string funName = __PRETTY_FUNCTION__;
            throw std::runtime_error(funName + ": Mock image could not be opened");
        }

        active.store(true);
        thread = std::thread(&MockCapture::update, this);
    }

    void MockCapture::stop()
    {
        active.store(false);
    }

    void MockCapture::update()
    {
        cv::Mat tmpFrame;
        while(active.load())
        {
            tmpFrame = img.clone();

            if(!tmpFrame.empty())
            {
                updateMutex.lock();
                if(transMat.empty())
                {
                    curFrame = tmpFrame;
                } else {
                    cv::resize(curFrame, curFrame, cv::Size(static_cast<int>(fabs(targetROI[1].x - targetROI[0].x)), static_cast<int>(fabs(targetROI[2].y - targetROI[1].y))));
                    cv::warpPerspective(tmpFrame, curFrame, transMat, curFrame.size());
                }
                updateMutex.unlock();
            }
        }
    }

    void MockCapture::setROI(std::array<cv::Point2f, 4>& roi, cv::Point2f::value_type width, cv::Point2f::value_type height)
    {
        targetROI[0].x = 0;
        targetROI[0].y = 0;
        targetROI[1].x = width;
        targetROI[1].y = 0;
        targetROI[2].x = width;
        targetROI[2].y = height;
        targetROI[3].x = 0;
        targetROI[3].y = height;
        transMat = cv::getPerspectiveTransform(roi, targetROI);
    }

    cv::Mat MockCapture::getFrame()
    {
        std::lock_guard<std::mutex> guard(updateMutex);
        return curFrame;
    }
    

    MockCapture::~MockCapture()
    {
        stop();
        thread.join();
    }
} // namespace ImageCapture