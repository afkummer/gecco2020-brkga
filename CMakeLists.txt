# General information.
cmake_minimum_required(VERSION 3.4)

project(gecco2020_brkga)

include_directories(src)
add_definitions(-Wall -Wextra -std=c++11 -pipe -march=x86-64 -mtune=generic -Wshadow -fopenmp)

# Debug and Release definitions.
# To set the build mode, specify the value of var `CMAKE_BUILD_TYPE` when
# running CMake.
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")

# Fallback to Debug build type automatically
# if no compilation mode was specified.
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Debug")
endif()

message("-- Using compilation flags of mode \"" ${CMAKE_BUILD_TYPE} "\"")


add_library(commons
   src/Instance.cpp
   src/Solution.cpp
   src/SortingDecoder.cpp
   src/Task.cpp
   src/brkga/Population.cpp
)
target_link_libraries(commons -fopenmp)

add_executable(brkga src/mainBrkga.cpp)
target_link_libraries(brkga commons)
