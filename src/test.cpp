#include <1_LL/network/network.h>
#include <2_DRIVERS/modbus/modbus.h>
#include <2_DRIVERS/modbus/modbus_sma.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define UNIT_IDENTIFIER 3
#define TIMEOUT 500

void pduExceptionHandler(uint16_t transactionNumber, modbusPduExceptionResponseHeader_t exceptionHeader);
void pduCb(uint16_t transactionNumber, uint16_t registerValue);
void printBufferHex(const uint8_t* pBuf, size_t length);

int main(int argc, char* argv[])
{

    /* Convert starting address & count */
    const uint16_t startingAddress = 30001;
    const uint16_t count = 1;
    const char* c_ip = "172.19.30.0";

    uint8_t buffer[eModbusMbapHeaderLength + eModbusPduRequestHeaderLength];

    /* Construct PDU */
    int pduLength = modbus_encodePdu(buffer, eModbusFunctionCode_readHoldingRegister, startingAddress, count);

    /* Construct MBAP */
    int mbapLength = modbus_encodeMbap(buffer, 0, pduLength, UNIT_IDENTIFIER);
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
    uint8_t responseBuffer[eModbus_maximumPduSizeTcp];
    uint16_t responseSize = eModbusMbapHeaderLength + eModbusPduResponseReadHoldingRegisterHeaderLength + (count * 2);

    error_e ret = network_readUntil(handle, responseBuffer, &responseSize);
    if (responseSize == 0 && eError_ok != ret) {
        perror("Couldn't read response");
        network_close(handle);
        network_deinit(handle);
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
        network_close(handle);
        network_deinit(handle);
        return EXIT_FAILURE;
    }

CLEANUP:
    network_close(handle);
    network_deinit(handle);

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