cmake_minimum_required(VERSION 3.10)

project(boundary_detect)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_definitions("-Wextra -Wconversion")

include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS})
include_directories(./)
include_directories(../)

set(SRC 
    src/Acquisition.cpp 
    src/BoundaryDetector.cpp
    src/BoundaryEnhancement.cpp
    src/BoundaryFeatureExtract.cpp
    src/BoundarySegmentation.cpp
    src/BoundaryClassifier.cpp
    src/CueDetector.cpp
    src/CueEnhancement.cpp
    src/CueFeatureExtraction.cpp
    src/CueSegmentation.cpp
    src/CueClassification.cpp
)

add_executable(${PROJECT_NAME}_test src/test.cpp)

add_library(${PROJECT_NAME} SHARED ${SRC})

if(WIN32)
    target_link_libraries(${PROJECT_NAME}_test ${OpenCV_LIBRARIES} ${PROJECT_NAME} image_cap)
    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES} image_cap)
    add_custom_command(TARGET ${PROJECT_NAME} COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.dll C:/lib)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME}_test ${OpenCV_LIBRARIES} ${PROJECT_NAME} image_cap pthread)
    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES} image_cap pthread)
endif()
