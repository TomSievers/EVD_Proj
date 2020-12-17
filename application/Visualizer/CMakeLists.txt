cmake_minimum_required(VERSION 3.10)

project(visualizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_definitions("-Wextra -Wconversion")

include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS})
include_directories(./)
include_directories(../)

set(SRC 
    src/ObjectVisualizer.cpp 
)

add_executable(${PROJECT_NAME}_test src/test.cpp ${SRC})

add_library(${PROJECT_NAME} SHARED ${SRC})

if(WIN32)
    target_link_libraries(${PROJECT_NAME}_test ${OpenCV_LIBRARIES})
    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES})
    add_custom_command(TARGET ${PROJECT_NAME} COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.dll C:/lib)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME}_test ${OpenCV_LIBRARIES} pthread)
    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES} pthread)
endif()
