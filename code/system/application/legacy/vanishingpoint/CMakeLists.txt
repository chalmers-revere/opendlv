cmake_minimum_required(VERSION 3.5)

project(vanishingpoint-rewrite)

set(OPENDAVINCI_DIR /opt/od4)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wconversion")

include_directories(include)
include_directories(${OPENDAVINCI_DIR}/include)
link_directories(${OPENDAVINCI_DIR}/lib)

add_subdirectory(apps)
