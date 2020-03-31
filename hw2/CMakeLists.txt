cmake_minimum_required(VERSION 3.14)
project(linedrawer)

set(CMAKE_CXX_STANDARD 14)

add_executable(linedrawer main.cpp)

target_sources(linedrawer PUBLIC GammaCorrection.h GammaCorrection.cpp)
target_sources(linedrawer PUBLIC Line.h Line.cpp)
target_sources(linedrawer PUBLIC Picture.h Picture.cpp)
target_sources(linedrawer PUBLIC Pixel.h Pixel.cpp)
target_sources(linedrawer PUBLIC Rectangle.h Rectangle.cpp)

if (CMAKE_BUILD_TYPE MATCHES Release)
    SET(GCC_COVERAGE_COMPILE_FLAGS "-O2")
endif ()