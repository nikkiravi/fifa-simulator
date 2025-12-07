cmake_minimum_required(VERSION 4.0)
project(fifa_simulator)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(include/models)

add_executable(fifa_simulator
        include/models/Player.h
        include/models/Player.h
        src/models/Player.cpp
        include/models/AttackerPlayer.h
        src/models/AttackPlayer.cpp
        include/models/GKPlayer.h
        src/models/GKPlayer.cpp
        include/models/DefenderPlayer.h
        include/models/MidfielderPlayer.h
        src/models/MidfielderPlayer.cpp
        src/models/DefenderPlayer.cpp
        src/main.cpp)
