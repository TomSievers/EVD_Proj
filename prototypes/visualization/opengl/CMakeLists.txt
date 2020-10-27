cmake_minimum_required(VERSION 3.5)

project(opengl_test)

set(OpenGL_GL_PREFERENCE "GLVND")

find_package(OpenGL REQUIRED)

if(NOT ${OpenGL_EGL_FOUND})
    message(SEND_ERROR "EGL NOT FOUND; INSTALL EGL")
else()

include_directories(${OPENGL_EGL_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})

add_executable(opengl_test main.c)

target_link_libraries(opengl_test OpenGL::OpenGL OpenGL::EGL)

endif()