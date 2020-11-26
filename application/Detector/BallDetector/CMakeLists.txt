cmake_minimum_required(VERSION 3.10)

project(ball_detector)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_definitions("-Wextra -Wconversion")

include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS})
include_directories(./)
include_directories(../common)
include_directories(${CMAKE_SOURCE_DIR}/ImageCapture)

set(SRC 
    src/BallAcquisition.cpp 
    src/BallClassification.cpp
    src/BallDetector.cpp 
    src/BallEnhancement.cpp 
    src/BallFeatureExtraction.cpp
    src/BallSegmentation.cpp
)

add_library(${PROJECT_NAME} SHARED ${SRC})

add_executable(${PROJECT_NAME}_test src/BallDetectorTest.cpp ${SRC})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES} image_cap)
target_link_libraries(${PROJECT_NAME}_test ${OpenCV_LIBRARIES} image_cap)