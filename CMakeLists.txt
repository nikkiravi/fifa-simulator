cmake_minimum_required(VERSION 3.15)

# Set vcpkg toolchain before project()
set(CMAKE_TOOLCHAIN_FILE "/home/nikkiravi/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project(FIFASimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(fifa_simulator ${SOURCES})

# Link libraries
target_link_libraries(fifa_simulator
    PRIVATE
    cpr::cpr
    nlohmann_json::nlohmann_json
)
