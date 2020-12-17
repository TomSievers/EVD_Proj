cmake_minimum_required(VERSION 3.10)

project(image_drawer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

find_package(PkgConfig)

if(${PKG_CONFIG_FOUND})
    pkg_check_modules(cairo REQUIRED cairo)
    add_definitions("-DHAVE_CAIRO")
elseif(NOT ${PKG_CONFIG_FOUND} AND UNIX)
    message("pkg-config not found cannot build with cairo")
else()
    message("pkg-config not found cannot build with cairo")
endif()

add_definitions("-Wextra -Wconversion")

include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS} ${cairo_INCLUDE_DIRS})
include_directories(./)

set(SRC 
    src/CairoDrawer.cpp 
)

add_executable(${PROJECT_NAME}_test src/test.cpp)

add_library(${PROJECT_NAME} SHARED ${SRC})

if(WIN32)
    target_link_libraries(${PROJECT_NAME}_test ${OpenCV_LIBRARIES} ${PROJECT_NAME})
    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES})
    add_custom_command(TARGET ${PROJECT_NAME} COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.dll C:/lib)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME}_test ${OpenCV_LIBRARIES} ${PROJECT_NAME} pthread)
    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES} ${cairo_LIBRARIES} pthread)
endif()