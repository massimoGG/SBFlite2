#include <1_LL/network/network.h>
#include <2_DRIVERS/modbus/modbus.h>
#include <3_APPLICATION/modbus/sma.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define UNIT_IDENTIFIER 3
#define TIMEOUT 500

bool g_debug = true;

void pduExceptionHandler(uint16_t transactionNumber, modbusPduExceptionResponseHeader_t exceptionHeader);
void pduCb(uint16_t transactionNumber, uint16_t registerValue);

int main(int argc, char* argv[])
{

    /* Convert starting address & count */
    const uint16_t startingAddress = 30775;
    const uint16_t count = 2; // 2 registers (U32)

    uint8_t buffer[eModbusMbapHeaderLength + eModbusPduRequestHeaderLength];

    /* Construct PDU */
    int pduLength = modbus_encodePdu(buffer, eModbusFunctionCode_readHoldingRegister, startingAddress, count);

    /* Construct MBAP */
    int mbapLength = modbus_encodeMbap(buffer, 0, pduLength, UNIT_IDENTIFIER);
    printBufferHex(buffer, mbapLength);

    /* Connect to inverter */
    networkHandle_t* handles[2] = {
        network_init(TIMEOUT),
        network_init(TIMEOUT)
    };

    if (eError_ok != network_connect(handles[0], "172.19.30.0", eModbus_port)) {
        perror("Couln't connect to inverter");
        network_deinit(handles[0]);
        return EXIT_FAILURE;
    }

    if (eError_ok != network_connect(handles[1], "172.19.40.0", eModbus_port)) {
        perror("Couln't connect to inverter");
        network_deinit(handles[1]);
        return EXIT_FAILURE;
    }

    for (;;) {

        for (int idx = 0; idx < 2; idx++) {
            networkHandle_t* pHandle = handles[idx];

            /* Send request */
            if (eError_ok != network_write(pHandle, buffer, mbapLength)) {
                perror("Couldn't send request");
                network_close(pHandle);
                network_deinit(pHandle);
                return EXIT_FAILURE;
            }

            /* Read */
            uint8_t responseBuffer[eModbus_maximumPduSizeTcp];
            uint16_t responseSize = eModbusMbapHeaderLength + eModbusPduResponseReadHoldingRegisterHeaderLength + (count * 2);

            error_e ret = network_readUntil(pHandle, responseBuffer, &responseSize);
            if (responseSize == 0 && eError_ok != ret) {
                perror("Couldn't read response");
                network_close(pHandle);
                network_deinit(pHandle);
                return EXIT_FAILURE;
            } else if (responseSize > 0 && eError_timeout == ret) {
                /* We did actually receive data, but it was not the expected size -> Possibly an exception code */
                printf("Received possible exception\n");
            }

            printf("Received %d bytes\n", responseSize);
            printBufferHex(responseBuffer, responseSize);

            /* Decode response */
            modbusDecodeCbs_t callbacks = {
                .exceptionCallback = pduExceptionHandler,
                .readHoldingRegisterCallback = pduCb,
            };

            ret = modbus_decode(responseBuffer, responseSize, &callbacks);
            if (eError_ok != ret) {
                fprintf(stderr, "Invalid MODBUS response %X\n", ret);
                network_close(pHandle);
                network_deinit(pHandle);
                return EXIT_FAILURE;
            }
        }

        sleep(1);
    }

CLEANUP:

    for (int idx = 0; idx < sizeof(handles); idx++) {
        networkHandle_t* pHandle = handles[idx];

        network_close(pHandle);
        network_deinit(pHandle);
    }

    return EXIT_SUCCESS;
}

/**
 * @brief exception handler
 * @param transactionNumber
 * @param exceptionHeader
 */
void pduExceptionHandler(uint16_t transactionNumber, modbusPduExceptionResponseHeader_t exceptionHeader)
{
    fprintf(stderr, "Transaction %d\tReceived exception PDU (%X %X) \n",
        transactionNumber, exceptionHeader.functionCode, exceptionHeader.exceptionCode);
}

void pduCb(uint16_t transactionNumber, uint16_t registerValue)
{
    fprintf(stdout, "Transaction %d\tRegister Value is %02X = %dd\n", transactionNumber, registerValue, registerValue);
}
