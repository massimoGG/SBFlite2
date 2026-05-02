cmake_minimum_required(VERSION 3.20)

project(SBFlite LANGUAGES C CXX)

# For VScode
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

add_executable(SBFlite)

target_sources(SBFlite
    PUBLIC
    ./src/main.cpp

    ./src/modbus/modbus.cpp

    ./src/inverter/inverter.cpp

    ./src/database/influx.cpp
    ./src/database/influxline.cpp
)

include_directories(SBFlite
    src/
)