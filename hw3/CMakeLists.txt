cmake_minimum_required(VERSION 3.14)
project(hw3)

set(CMAKE_CXX_STANDARD 14)

add_executable(dither main.cpp)

target_sources(dither PUBLIC GammaCorrection.h GammaCorrection.cpp)
target_sources(dither PUBLIC Picture.h Picture.cpp)
target_sources(dither PUBLIC Dither.h Dither.cpp)