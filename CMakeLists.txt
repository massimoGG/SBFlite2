cmake_minimum_required(VERSION 3.20)

project(SBFlite LANGUAGES C CXX)

# For VScode
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

add_executable(SBFlite)

target_compile_options(SBFlite
    PUBLIC
    -Wall -Wno-exceptions
)

target_sources(SBFlite
    PUBLIC
    ./src/config/config.cpp

    ./src/database/influx.cpp
    ./src/database/influxline.cpp

    ./src/inverter/inverter.cpp

    ./src/modbus/modbus.cpp
    ./src/modbus/modbus.c

    ./src/main.cpp
)

include_directories(SBFlite
    src/
    inc/
    submodules/toml11/include
)
