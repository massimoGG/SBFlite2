cmake_minimum_required(VERSION 3.20)

# Find libcurl
find_package(
    CURL 
    REQUIRED
)

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

    ./src/2_DRIVERS/modbus/modbus_mbap.c
    ./src/2_DRIVERS/modbus/modbus_pdu.c
    ./src/2_DRIVERS/modbus/modbus.c

    ./src/3_APPLICATION/config/config.cpp
    ./src/3_APPLICATION/inverter/inverter.cpp
    ./src/3_APPLICATION/modbus/sma.c

    ./src/main.cpp
)
include_directories(SBFlite
    src/
    inc/
    submodules/toml11/include
)
target_link_libraries(
    SBFlite
    PRIVATE
    CURL::libcurl
)

add_executable(mbcli)
target_sources(mbcli
    PUBLIC

    ./src/1_LL/network/network.c

    ./src/2_DRIVERS/modbus/modbus.c
    ./src/2_DRIVERS/modbus/modbus_mbap.c
    ./src/2_DRIVERS/modbus/modbus_pdu.c

    ./src/3_APPLICATION/inverter/inverter.cpp
    ./src/3_APPLICATION/modbus/sma.c

    ./src/cli.cpp

)
include_directories(mbcli
    src/
    inc/
)
target_link_libraries(
    mbcli
    PRIVATE
    CURL::libcurl
)

add_executable(test)
target_sources(test
    PUBLIC

    ./src/1_LL/network/network.c

    ./src/2_DRIVERS/modbus/modbus.c
    ./src/2_DRIVERS/modbus/modbus_mbap.c
    ./src/2_DRIVERS/modbus/modbus_pdu.c

    ./src/3_APPLICATION/inverter/inverter.cpp
    ./src/3_APPLICATION/modbus/sma.c

    ./src/test.cpp

)
include_directories(test
    src/
    inc/
)
target_link_libraries(
    test
    PRIVATE
    CURL::libcurl
)

