/**
 * @file modbus_pdu.c
 * @brief MB PDU decode/encode implementations
 */

#include "modbus_pdu.h"
#include "modbus_mbap.h"
#include <arpa/inet.h> // for htons and ntohs
#include <string.h>

/**
 * @brief encodes the PDU on \p pBuf
 * @param pBuf              pointer to starting address of the whole packet's buffer
 * @param code              function code to encode
 * @param startingAddress   starting address of register
 * @param quantity          quantity of registers
 * @returns int             length of PDU
 * @retval -1               on error
 */
int modbus_encodePdu(uint8_t* pBuf, modbusFunctionCodes_e code, uint16_t startingAddress, uint16_t quantity)
{
    const modbusPduRequestHeader_t header = {
        .functionCode = code,
        .startingAddress = htons(startingAddress),
        .quantity = htons(quantity),
    };

    memcpy(pBuf + eModbusMbapHeaderLength, &header, eModbusPduRequestHeaderLength);

    return eModbusPduRequestHeaderLength;
}

/**
 * @brief decodes the PDU header on \p pBuf
 * @param pBuf                  pointer to the whole packet
 * @param length                length of the buffer
 * @param pHeader               pointer to the PDU header struct
 * @retval eError_ok            valid PDU, \p pHeader contains the PDU header
 * @retval eError_invalidLength invalid buffer length
 */
error_e modbus_decodePdu(const uint8_t* pBuf, uint16_t length, modbusPduResponseHeader_t* pHeader)
{
    if (length < eModbusMbapHeaderLength + eModbusPduResponseHeaderLength) {
        return eError_invalidLength;
    }

    modbusPduResponseHeader_t header = {};

    memcpy(&header, pBuf + eModbusMbapHeaderLength, eModbusPduResponseHeaderLength);

    *pHeader = header;

    return eError_ok;
}

/**
 * @brief gets the function code in the given \p pHeader
 * @param pHeader                   pointer to PDU header
 * @return modbusFunctionCodes_e    value
 */
modbusFunctionCodes_e modbus_getPduFunctionCode(modbusPduResponseHeader_t* pHeader)
{
    return pHeader->code;
}

/**
 * @brief decodes the exception PDU header on \p pBuf
 * @param pBuf                  pointer to the whole packet
 * @param length                length of the buffer
 * @param pHeader               pointer to the PDU exception header
 * @retval eError_ok            successfully decoded
 * @retval eError_invalidLength invalid buffer length
 */
error_e modbus_decodePduException(const uint8_t* pBuf, uint16_t length, modbusPduExceptionResponseHeader_t* pHeader)
{
    if (length < eModbusMbapHeaderLength + eModbusPduResponseExceptionHeaderLength) {
        return eError_invalidLength;
    }

    modbusPduExceptionResponseHeader_t header = {};

    memcpy(&header, pBuf + eModbusMbapHeaderLength, eModbusPduResponseExceptionHeaderLength);

    *pHeader = header;

    return eError_ok;
}

/**
 * @brief decodes the read holding registers PDU header on \p pBuf
 * @param pBuf                  pointer to the whole packet
 * @param length                length of the buffer
 * @param pHeader               pointer to the PDU header
 * @retval eError_ok            successfully decoded
 * @retval eError_invalidLength invalid buffer length
 */
error_e modbus_decodePduReadHoldingRegisterHeader(const uint8_t* pBuf, uint16_t length, modbusPduResponseReadHoldingRegistersHeader_t* pHeader)
{
    if (length < eModbusMbapHeaderLength + eModbusPduResponseReadHoldingRegisterHeaderLength) {
        return eError_invalidLength;
    }

    modbusPduResponseReadHoldingRegistersHeader_t header = {};

    memcpy(&header, pBuf + eModbusMbapHeaderLength, eModbusPduResponseReadHoldingRegisterHeaderLength);

    *pHeader = header;

    return eError_ok;
}

/**
 * @brief gets the next valid register from response
 * @param[in] pBuf                  pointer to the whole packet
 * @param[in] bufferLength          length of the buffer
 * @param[in] pHeader               pointer to the PDU header
 * @param[in,out] pByteCountOffset  offset used for this function, initialize with 0!
 * @param[out] pValue               the register value
 * @retval eError_ok                successfully got next available register
 * @retval eError_noData            no more register left to get
 * @retval eError_invalidLength     invalid buffer length
 */
error_e modbus_getNextRegister(const uint8_t* pBuf, uint16_t bufferLength, modbusPduResponseReadHoldingRegistersHeader_t* pHeader, uint8_t* pByteCountOffset, uint16_t* pValue)
{
    if (bufferLength < eModbusMbapHeaderLength + eModbusPduResponseReadHoldingRegisterHeaderLength + pHeader->byteCount) {
        return eError_invalidLength;
    }

    const uint16_t c_offset = eModbusMbapHeaderLength + eModbusPduResponseReadHoldingRegisterHeaderLength;
    const uint8_t c_registerSize = 2U;

    if (*pByteCountOffset >= pHeader->byteCount) {
        /* Reached the end of the PDU buffer */
        return eError_noData;
    }

    const uint8_t* pRegisterOffset = pBuf + c_offset + *pByteCountOffset;

    /* Convert MSB & LSB */
    const uint16_t registerValue = *(pRegisterOffset) + (*(pRegisterOffset + 1) << 8);

    *pByteCountOffset += c_registerSize;
    *pValue = ntohs(registerValue);

    return eError_ok;
}