#ifndef BALL_OBJECT_HPP
#define BALL_OBJECT_HPP

#include "include/Object.hpp"
#include <opencv2/core/types.hpp>
#include <cstdint>

namespace Detector
{
    enum BallType
    {
        CUE_BALL,
        NOT_CUE_BALL
    };

    struct BallObject : public Object
    {
        uint8_t percentageWhite;
        cv::Point point;
        float radius;
        BallType ballType;
    };
}

#endif