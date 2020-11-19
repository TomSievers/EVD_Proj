cmake_minimum_required(VERSION 3.10)

project(pool_aid)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

file(GLOB children RELATIVE ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/*)

foreach(child ${children})
    if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/${child})
        if(NOT ${child} STREQUAL "build")
            add_subdirectory(${CMAKE_SOURCE_DIR}/${child})
        endif()
    endif()
endforeach()

add_definitions("-Wextra -Wconversion")

include_directories(${OpenCV_INCLUDE_DIRS} ./)

add_executable(${PROJECT_NAME} main.cpp)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES} image_cap)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES} pthread image_cap)
endif()

