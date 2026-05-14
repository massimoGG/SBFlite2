#include <1_LL/network/network.h>
#include <2_DRIVERS/modbus/modbus.h>
#include <2_DRIVERS/modbus/modbus_sma.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define TIMEOUT 3000

void printBufferHex(const uint8_t* pBuf, size_t length);

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

    uint8_t buffer[c_modbusMbapHeaderLength + c_modbusPduRequestHeaderLength];

    /* Construct PDU */
    int pduLength = modbus_encodePdu(buffer, eModbusFunctionCode_readHoldingRegister, startingAddress, count);

    /* Construct MBAP */
    int mbapLength = modbus_encodeMbap(buffer, 123, pduLength);
    printBufferHex(buffer, mbapLength);

    /* Connect to inverter */
    networkHandle_t* handle = network_init(TIMEOUT);

    if (eError_ok != network_connect(handle, c_ip, eModbus_port)) {
        perror("Couln't connect to inverter");
        network_deinit(handle);
        return EXIT_FAILURE;
    }

    /* Send request */
    if (eError_ok != network_write(handle, buffer, mbapLength)) {
        perror("Couldn't send request");
        network_close(handle);
        network_deinit(handle);
        return EXIT_FAILURE;
    }

    /* Read */
    uint8_t responseBuffer[eModbus_replyMaximumSizeTcp];
    uint16_t responseSize = sizeof(responseBuffer);

    error_e ret = network_read(handle, responseBuffer, &responseSize);
    if (eError_ok != ret) {
        perror("Couldn't read response");
        network_close(handle);
        network_deinit(handle);
        return EXIT_FAILURE;
    }

    printf("Received %d bytes\n", responseSize);
    printBufferHex(responseBuffer, responseSize);

    /* Print buffer */

    network_deinit(handle);

    return EXIT_SUCCESS;
}

/**
 * @brief prints the given buffer in hex
 * @param pBuf      pointer to buffer
 * @param length    length of buffer
 */
void printBufferHex(const uint8_t* pBuf, size_t bufferLength)
{
    const size_t lineLength = 16U;
    /* Format per 16 bytes */
    for (size_t lineOffset = 0; lineOffset < bufferLength; lineOffset += lineLength) {
        /* For each line print the hexadecimal */
        for (size_t idx = lineOffset; (idx < lineOffset + lineLength) && (idx < bufferLength); idx++) {
            fprintf(stdout, "%02X ", pBuf[idx]);
        }
        fprintf(stdout, "\t");
        /* Print the character */
        for (size_t idx = lineOffset; (idx < lineOffset + lineLength) && (idx < bufferLength); idx++) {
            const char character = pBuf[idx];
            if ((character > 32) && (character < 126)) {
                fprintf(stdout, "%c ", character);
            } else {
                fprintf(stdout, ".  ");
            }
        }
        fprintf(stdout, "\n");
    }
}