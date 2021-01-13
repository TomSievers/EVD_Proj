cmake_minimum_required(VERSION 3.10)

project(user_interface)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_definitions("-Wextra -Wconversion -Wno-psabi")

include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS})
include_directories(./)
include_directories(../)

set(SRC 
    src/KeyboardInterface.cpp
)

add_executable(${PROJECT_NAME}_test src/test.cpp)

add_library(${PROJECT_NAME} SHARED ${SRC})

if(WIN32)
    target_link_libraries(${PROJECT_NAME}_test ${OpenCV_LIBRARIES} ${PROJECT_NAME} ws2_32)
    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES} ws2_32)
    add_custom_command(TARGET ${PROJECT_NAME} COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.dll C:/lib)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME}_test ${OpenCV_LIBRARIES} ${PROJECT_NAME} ${ncurses_LIBRARIES} pthread)
    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES} ${ncurses_LIBRARIES} pthread) 
endif()
