cmake_minimum_required(VERSION 3.10)
project(garp LANGUAGES C)

include_directories(src)

add_executable(${CMAKE_PROJECT_NAME} src/garp.c src/main.c)