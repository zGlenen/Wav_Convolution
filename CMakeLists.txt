cmake_minimum_required(VERSION 3.21)
project(testest)

set(CMAKE_CXX_STANDARD 17)

add_executable(testest src/main.cpp src/AudioFFT.cpp src/Utilities.cpp src/FFTConvolver.cpp src src/wavs.c)
