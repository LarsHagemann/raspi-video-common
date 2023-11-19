cmake_minimum_required(VERSION 3.15)
project(raspi-video-common)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT TARGET Catch2)
  add_subdirectory(Catch2)
endif()

enable_testing()

add_subdirectory(src)

