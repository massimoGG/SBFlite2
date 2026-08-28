
#include "sma.h"
#include <stdbool.h>
#include <stdio.h>

/** Globals */
extern bool g_debug;

/**
 * @brief sends a MB request for register \p startingAddress \p count long
 * @param pHandle
 * @param startingAddress
 * @param count
 * @return error_e
 */
error_e sendReadHoldingRegister(modbusWrapper_handle_t* pHandle, uint16_t startingAddress, uint16_t count)
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

/**
 * @brief reads a MB response
 * @param pHandle
 * @param count
 * @param[out] pHoldingHeader   pointer to response struct
 * @retval eError_ok            received succesfully the Read Holding Register
 * @retval eError_failed        a network error occurred
 * @retval eError_invalid       received an exception
 * @retval eError_invalidData   received invalid response
 */
error_e fetchReadHoldingRegister(modbusWrapper_handle_t* pHandle, uint8_t count, modbusPduResponseReadHoldingRegistersHeader_t* pHoldingHeader)
{
    /* The expected response size */
    pHandle->responseSize = eModbusMbapHeaderLength + eModbusPduResponseReadHoldingRegisterHeaderLength + (count * 2);

    error_e ret = network_readUntil(pHandle->pNetworkHandle, pHandle->responseBuffer, &pHandle->responseSize);

    if (pHandle->responseSize == 0 && eError_ok != ret) {
        perror("Couldn't read response");
        return eError_failed;
    } else if (pHandle->responseSize > 0 && eError_timeout == ret) {
        /* We did actually receive data but it was not the expected size -> Possibly an exception code */
        printf("Received possible exception\n");
    }

    if (g_debug) {
        printf("Received %d bytes\n", pHandle->responseSize);
        printBufferHex(pHandle->responseBuffer, pHandle->responseSize);
    }

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

    if (pdu.code != eModbusFunctionCode_readHoldingRegister) {
        return eError_invalidData;
    }

    if (eError_ok != modbus_decodePduReadHoldingRegisterHeader(pHandle->responseBuffer, pHandle->responseSize, pHoldingHeader)) {
        return eError_invalidData;
    }

    return eError_ok;
}

/**
 * @brief fetches a U32 register
 * @param pHandle
 * @param address
 * @param[out] pValue           pointer to the U32
 * @retval eError_invalidData   invalid response
 * @retval eError_ok            successfully decoded U32
 */
error_e fetchU32(modbusWrapper_handle_t* pHandle, uint16_t address, uint32_t* pValue)
{
    /* Send request */
    error_e ret = sendReadHoldingRegister(pHandle, address, 2);
    if (eError_ok != ret) {
        return ret;
    }

    /* Read response */
    modbusPduResponseReadHoldingRegistersHeader_t holdingHeader = {};

    ret = fetchReadHoldingRegister(pHandle, 2, &holdingHeader);
    if (eError_ok != ret) {
        return ret;
    }

    uint8_t offset = 0;
    uint16_t registerValue = 0;

    /* Read the first half */
    if (eError_ok != modbus_getNextRegister(pHandle->responseBuffer, pHandle->responseSize, &holdingHeader, &offset, &registerValue)) {
        return eError_invalidData;
    }
    *pValue = registerValue << 16U;

    if (eError_ok != modbus_getNextRegister(pHandle->responseBuffer, pHandle->responseSize, &holdingHeader, &offset, &registerValue)) {
        // printf("%s\t%d -> %d\n", __func__, offset, registerValue);
        return eError_invalidData;
    }
    *pValue |= registerValue;

    return eError_ok;
}

/**
 * @brief fetches a U32 register
 * @param pHandle
 * @param address
 * @param count                 number of U32 registers
 * @param[in,out] pRegisters    pointer to where the registers will be stored
 * @retval eError_invalidData   invalid response
 * @retval eError_ok            successfully decoded U32
 */
error_e fetchU32Multiple(modbusWrapper_handle_t* pHandle, uint16_t address, uint16_t count, uint32_t* pRegisters)
{
    /* Send request */
    error_e ret = sendReadHoldingRegister(pHandle, address, count * 2);
    if (eError_ok != ret) {
        return ret;
    }

    /* Read response */
    modbusPduResponseReadHoldingRegistersHeader_t holdingHeader = {};

    ret = fetchReadHoldingRegister(pHandle, count * 2, &holdingHeader);
    if (eError_ok != ret) {
        return ret;
    }

    uint8_t offset = 0;

    for (uint16_t registerValue = 0; count; count--) {
        /* Get first half */
        if (eError_ok != modbus_getNextRegister(pHandle->responseBuffer, pHandle->responseSize, &holdingHeader, &offset, &registerValue)) {
            return eError_invalidData;
        }
        uint32_t value = registerValue << 16U;

        /* Get second half */
        if (eError_ok != modbus_getNextRegister(pHandle->responseBuffer, pHandle->responseSize, &holdingHeader, &offset, &registerValue)) {
            return eError_invalidData;
        }
        value |= registerValue;

        /* Store at given array */
        *pRegisters++ = value;
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