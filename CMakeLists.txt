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

    ./src/1_LL/network/network.c

    ./src/2_DRIVERS/database/influx.cpp
    ./src/2_DRIVERS/database/influxline.cpp

    ./src/2_DRIVERS/modbus/modbus.cpp

    ./src/config/config.cpp
    ./src/inverter/inverter.cpp

    ./src/main.cpp
)
include_directories(SBFlite
    src/
    inc/
    submodules/toml11/include
)

add_executable(mbcli)
target_sources(mbcli
    PUBLIC

    ./src/1_LL/network/network.c

    ./src/2_DRIVERS/modbus/modbus.c
    ./src/2_DRIVERS/modbus/modbus_mbap.c
    ./src/2_DRIVERS/modbus/modbus_pdu.c

    ./src/inverter/inverter.cpp

    ./src/cli.cpp

)
include_directories(mbcli
    src/
    inc/
)

add_executable(test)
target_sources(test
    PUBLIC

    ./src/1_LL/network/network.c

    ./src/2_DRIVERS/modbus/modbus.c
    ./src/2_DRIVERS/modbus/modbus_mbap.c
    ./src/2_DRIVERS/modbus/modbus_pdu.c

    ./src/inverter/inverter.cpp

    ./src/test.cpp

)
include_directories(test
    src/
    inc/
)

