
#include "modbus_wrapper.h"
#include <stdio.h>

error_e sendRequest(modbusWrapper_handle_t* pHandle, uint16_t startingAddress, uint16_t count)
{
    /* Request buffer */
    uint8_t buffer[eModbusMbapHeaderLength + eModbusPduRequestHeaderLength];

    /* Construct PDU */
    int pduLength = modbus_encodePdu(buffer, eModbusFunctionCode_readHoldingRegister, startingAddress, count);

    /* Construct MBAP */
    int mbapLength = modbus_encodeMbap(buffer, 0, pduLength, pHandle->unitIdentifier);

    /* Send request */
    return network_write(pHandle->pNetworkHandle, buffer, mbapLength);
}

error_e readResponse(modbusWrapper_handle_t* pHandle, uint8_t count)
{
    uint16_t responseExpectedSize = eModbusMbapHeaderLength + eModbusPduResponseReadHoldingRegisterHeaderLength + (count * 2);

    error_e ret = network_readUntil(pHandle->pNetworkHandle, pHandle->responseBuffer, &responseExpectedSize);
    pHandle->responseSize = responseExpectedSize;

    if (pHandle->responseSize == 0 && eError_ok != ret) {
        perror("Couldn't read response");
        return eError_failed;
    } else if (pHandle->responseSize > 0 && eError_timeout == ret) {
        /* We did actually receive data but it was not the expected size -> Possibly an exception code */
        printf("Received possible exception\n");
    }

    printf("Received %d bytes\n", pHandle->responseSize);
    printBufferHex(pHandle->responseBuffer, pHandle->responseSize);

    /* decode MBAP */
    modbusMbapHeader_t mbap = {};
    ret = modbus_decodeMbap(pHandle->responseBuffer, pHandle->responseSize, &mbap);
    if (eError_ok != ret) {
        return eError_invalidData;
    }

    /* decode PDU */
    modbusPduResponseHeader_t pdu = {};
    ret = modbus_decodePdu(pHandle->responseBuffer, pHandle->responseSize, &pdu);
    if (eError_ok != ret) {
        return eError_invalidData;
    }

    /* Determine whether it's an exception */
    if (pdu.code & eModbusFunctionCode_exceptionOffset) {

        modbusPduExceptionResponseHeader_t exceptionheader = {};

        if (eError_ok != modbus_decodePduException(pHandle->responseBuffer, pHandle->responseSize, &exceptionheader)) {
            return eError_invalidData;
        }

        return eError_invalid;
    }

    /* If not, decode based on returned function code */
    switch (pdu.code) {
    case eModbusFunctionCode_readCoils:
        break;
    case eModbusFunctionCode_readDiscreteInputs:
        break;
    case eModbusFunctionCode_readHoldingRegister:
        /* Convert to */
        modbusPduResponseReadHoldingRegistersHeader_t holdingHeader = {};

        if (eError_ok != modbus_decodePduReadHoldingRegisterHeader(pHandle->responseBuffer, pHandle->responseSize, &holdingHeader)) {
            return eError_invalidData;
        }

        uint8_t offset = 0;
        uint16_t registerValue = 0;
        while (eError_noData != modbus_getNextRegister(pHandle->responseBuffer, pHandle->responseSize, &holdingHeader, &offset, &registerValue)) {
            printf("%s\t%d -> %d\n", __func__, offset, registerValue);
        }

        break;
    }

    return eError_ok;
}

/**
 * @brief prints the given buffer in hex
 * @param pBuf      pointer to buffer
 * @param length    length of buffer
 */
void printBufferHex(const uint8_t* pBuf, uint32_t bufferLength)
{
    const uint32_t lineLength = 16U;
    /* Format per 16 bytes */
    for (uint32_t lineOffset = 0; lineOffset < bufferLength; lineOffset += lineLength) {
        /* For each line print the hexadecimal */
        for (uint32_t idx = lineOffset; (idx < lineOffset + lineLength) && (idx < bufferLength); idx++) {
            fprintf(stdout, "%02X ", pBuf[idx]);
        }
        fprintf(stdout, "\t");
        /* Print the character */
        for (uint32_t idx = lineOffset; (idx < lineOffset + lineLength) && (idx < bufferLength); idx++) {
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