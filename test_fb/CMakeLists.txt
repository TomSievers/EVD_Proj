cmake_minimum_required(VERSION 3.5)

project(fb)

add_executable(fb main.c)

target_link_libraries(fb util z)
