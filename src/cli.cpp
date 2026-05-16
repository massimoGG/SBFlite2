#include <1_LL/network/network.h>
#include <2_DRIVERS/modbus/modbus.h>
#include <3_APPLICATION/modbus/modbus_sma.h>
#include <3_APPLICATION/modbus/sma.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define UNIT_IDENTIFIER 3
#define TIMEOUT 500

bool g_debug = true;

int main(int argc, char* argv[])
{

    /* Fetch arguments */
    if (argc != 4) {
        fprintf(stdout, "Usage %s <IP> <Starting Address> <Counts>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* c_ip = argv[1];
    const char* c_startingAddress = argv[2];
    const char* c_count = argv[3];

    /* Convert starting address & count */
    const uint16_t startingAddress = strtol(c_startingAddress, NULL, 10);
    const uint16_t count = strtol(c_count, NULL, 10);

    /* Connect to inverter */
    networkHandle_t* handle = network_init(TIMEOUT);

    if (eError_ok != network_connect(handle, c_ip, eModbus_port)) {
        perror("Couln't connect to inverter");
        network_deinit(handle);
        return EXIT_FAILURE;
    }

    modbusWrapper_handle_t mbWrapperHandle = {
        .unitIdentifier = UNIT_IDENTIFIER,
        .pNetworkHandle = handle,
        .responseBuffer = "",
    };

    if (eError_ok != sendReadHoldingRegister(&mbWrapperHandle, startingAddress, count)) {
        perror("Couldn't send request");
        network_close(handle);
        network_deinit(handle);
        return EXIT_FAILURE;
    }

    modbusPduResponseReadHoldingRegistersHeader_t holdingHeader;
    if (eError_ok != fetchReadHoldingRegister(&mbWrapperHandle, count, &holdingHeader)) {
        perror("Couldn't send request");
        network_close(handle);
        network_deinit(handle);
        return EXIT_FAILURE;
    }

    uint8_t offset = 0;
    uint16_t registerValue = 0;

    uint32_t u32Value = 0;

    /* Read the first half */
    while (eError_noData != modbus_getNextRegister(mbWrapperHandle.responseBuffer, mbWrapperHandle.responseSize, &holdingHeader, &offset, &registerValue)) {
        printf("%d\n", registerValue);
        u32Value |= registerValue << (offset / 2 * 16);
    }
    printf("%d\n", u32Value);

CLEANUP:
    network_close(handle);
    network_deinit(handle);

    return EXIT_SUCCESS;
}
