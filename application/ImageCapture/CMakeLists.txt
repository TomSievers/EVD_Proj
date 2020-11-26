cmake_minimum_required(VERSION 3.10)

project(image_cap)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_definitions("-Wextra -Wconversion")

include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS})
include_directories(./)

set(SRC 
    src/CameraConfig.cpp 
    src/Capture.cpp
    src/MockCapture.cpp
)

add_executable(${PROJECT_NAME}_test src/test.cpp)

add_library(${PROJECT_NAME} SHARED ${SRC})

if(WIN32)
    target_link_libraries(${PROJECT_NAME}_test ${OpenCV_LIBRARIES} ${PROJECT_NAME})
    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES})
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME}_test ${OpenCV_LIBRARIES} ${PROJECT_NAME} pthread)
    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES} pthread)
endif()
