cmake_minimum_required(VERSION 3.10)

project(detector)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

foreach(child ${children})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child})
        if(NOT ${child} STREQUAL "build" AND NOT ${child} STREQUAL "common")
            add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${child})
        endif()
    endif()
endforeach()
