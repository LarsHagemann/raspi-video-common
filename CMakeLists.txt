cmake_minimum_required(VERSION 3.15)
project(raspi-video-common)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Catch2)

enable_testing()

add_subdirectory(src)

