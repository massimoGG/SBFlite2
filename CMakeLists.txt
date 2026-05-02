cmake_minimum_required(VERSION 3.20)

project(SBFlite LANGUAGES C CXX)

# For VScode
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

add_executable(SBFlite)

target_sources(SBFlite
    PUBLIC
    ./src/influx.cpp
    ./src/inverter.cpp
    ./src/line.cpp
    ./src/main.cpp
    ./src/modbus.cpp
)

include_directories(SBFlite
    src/
)