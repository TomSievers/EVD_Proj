cmake_minimum_required(VERSION 3.10)

project(pool_aid)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

find_package(OpenCV REQUIRED)

find_package(PkgConfig)

if(${PKG_CONFIG_FOUND})
    pkg_check_modules(cairo REQUIRED cairo)
    pkg_check_modules(ncurses REQUIRED ncurses)
    pkg_check_modules(libdrm REQUIRED libdrm)
    add_definitions("-DHAVE_CAIRO")
    add_definitions("-DHAVE_CURSES")
    add_definitions("-DHAVE_LIBDRM")
else()
    message("pkg-config not found cannot build with cairo")
endif()


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

