cmake_minimum_required(VERSION 3.10)

project(pool_aid)

find_package(OpenCV REQUIRED)

set(img_cap_src 
    ImageCapture/src/Capture.cpp 
    ImageCapture/src/CameraConfig.cpp
)

add_definitions("-Wextra -Wconversion")

include_directories(${OpenCV_INCLUDE_DIRS} ./)

add_executable(${PROJECT_NAME} main.cpp ${img_cap_src})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES})
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES} pthread)
endif()

