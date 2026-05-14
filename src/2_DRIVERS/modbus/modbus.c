/**
 * @file modbus.c
 * @brief the modbus implementation
 */

#include "modbus.h"
#include <arpa/inet.h> // for htons and ntohs
#include <string.h>

/**
 * @brief encodes the PDU on \p pBuf
 * @param pBuf      pointer to starting address of the whole packet's buffer
 * @returns int     length of PDU
 * @retval -1       on error
 */
int modbus_encodePdu(uint8_t* pBuf, modbusFunctionCodes_e code, uint16_t startingAddress, uint16_t quantity)
{

    /* Fill header with big-endian representation */
    const modbusPduRequestHeader_t header = {
        .functionCode = code,
        .startingAddress = htons(startingAddress),
        .quantity = htons(quantity),
    };

    memcpy(pBuf + c_modbusMbapHeaderLength, &header, c_modbusPduRequestHeaderLength);

    return c_modbusPduRequestHeaderLength;
}

/**
 * @brief encodes the MBAP header on \p pBuf
 * @param pBuf              pointer to starting address of the whole packet's buffer
 * @param transactionID     transaction ID
 * @param pduLength         total length of the PDU
 * @return int              total length of whole encoded packet
 */
int modbus_encodeMbap(uint8_t* pBuf, uint16_t transactionID, uint16_t pduLength)
{
    const modbusMbapHeader_t header = {
        .transactionIdentifier = htons(transactionID),
        /* 0 for MB */
        .protocolIdentifier = htons(0U),
        /* Length of PDU + unit identifier */
        .length = htons(pduLength + 1),
        /* 0xFF for TCP */
        .unitIdentifier = 0xFF,
    };

    memcpy(pBuf, &header, c_modbusMbapHeaderLength);

    return c_modbusMbapHeaderLength + pduLength;
}
