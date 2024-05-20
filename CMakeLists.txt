cmake_minimum_required(VERSION 3.16)
project(ShortestPath)


add_executable(ShortestPath
        src/main.cpp
        src/ShortestPath.cpp
        src/Algorithms.cpp
)
include_directories(${ShortestPath_SOURCE_DIR}/include)
target_link_libraries(ShortestPath PRIVATE sfml-system sfml-graphics sfml-audio sfml-window )
