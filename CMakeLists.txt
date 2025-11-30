cmake_minimum_required(VERSION 4.0)
project(fifa_simulator)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(include/models)

add_executable(fifa_simulator
        include/models/Player.h
        FIFA_Ultimate_Team_Simulator_Project.pdf
        include/models/Player.h)
