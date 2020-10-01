cmake_minimum_required(VERSION 3.5)

project(cairo_test)

find_package(PkgConfig REQUIRED)

pkg_check_modules(cairo REQUIRED cairo)

include_directories(${cairo_INCLUDE_DIRS})

add_executable(cairo_test main.c)

target_link_libraries(cairo_test ${cairo_LIBRARIES})