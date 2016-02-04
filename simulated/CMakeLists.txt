cmake_minimum_required(VERSION 3.3)
project(simulated)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp cawax_datastructs.h cawax_maths.cpp cawax_maths.h)
add_executable(simulated ${SOURCE_FILES})