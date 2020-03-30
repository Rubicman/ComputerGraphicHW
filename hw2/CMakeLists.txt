cmake_minimum_required(VERSION 3.14)
project(hw2)

set(CMAKE_CXX_STANDARD 14)

add_executable(hw2 main.cpp)

target_sources(hw2 PUBLIC GammaCorrection.h GammaCorrection.cpp)
target_sources(hw2 PUBLIC Line.h Line.cpp)
target_sources(hw2 PUBLIC Picture.h Picture.cpp)
target_sources(hw2 PUBLIC Pixel.h Pixel.cpp)
target_sources(hw2 PUBLIC Rectangle.h Rectangle.cpp)

if (CMAKE_BUILD_TYPE MATCHES Release)
    SET(GCC_COVERAGE_COMPILE_FLAGS "-O2")
endif ()