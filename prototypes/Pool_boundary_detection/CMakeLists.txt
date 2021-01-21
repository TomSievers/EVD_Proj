cmake_minimum_required(VERSION 3.10)

project(pool_aid)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_definitions("-Wall -Wextra -Wconversion")

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} main.cpp boundarydetector.cpp)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES})
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES} )
endif()