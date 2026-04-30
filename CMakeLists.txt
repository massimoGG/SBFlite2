cmake_minimum_required(VERSION 3.20)

project(SBFlite LANGUAGES C CXX)

add_executable(SBFlite)

target_sources(SBFlite
    PRIVATE
    ./src/influx.cpp
    ./src/line.cpp
    ./src/modbus.cpp
    ./src/main.cpp
)