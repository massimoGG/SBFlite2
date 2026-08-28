/**
 * @file modbus_mbap.c
 * @brief MB MBAP header implementation
 */

#include "modbus.h"
#include <arpa/inet.h>
#include <string.h>

/**
 * @brief encodes the MBAP header on \p pBuf
 * @param pBuf              pointer to starting address of the whole packet's buffer
 * @param transactionID     transaction ID
 * @param pduLength         total length of the PDU
 * @return int              total length of whole encoded packet
 */
int modbus_encodeMbap(uint8_t* pBuf, uint16_t transactionID, uint16_t pduLength, uint8_t unitIdentifier)
{
    const modbusMbapHeader_t header = {
        .transactionIdentifier = htons(transactionID),
        /* 0 for MB */
        .protocolIdentifier = htons(eModbus_protocolIdentifier),
        /* Length of PDU + unit identifier */
        .length = htons(pduLength + 1),
        /* 0xFF for TCP */
        .unitIdentifier = unitIdentifier,
    };

    memcpy(pBuf, &header, eModbusMbapHeaderLength);

    return eModbusMbapHeaderLength + pduLength;
}

/**
 * @brief decodes the MBAP header on \p pBuf
 * @param[in] pBuf              pointer to whole packet
 * @param[in] length            length of buffer
 * @param[out] pHeader          pointer to MBAP header struct
 * @retval eError_ok            valid MBAP, \p pHeader contains the MBAP header
 * @retval eError_invalid       invalid MBAP
 * @retval eError_invalidLength invalid buffer length
 */
error_e modbus_decodeMbap(const uint8_t* pBuf, uint16_t length, modbusMbapHeader_t* pHeader)
{
    if (length < eModbusMbapHeaderLength) {
        return eError_invalidLength;
    }

    modbusMbapHeader_t header;

    memcpy(&header, pBuf, eModbusMbapHeaderLength);

    /** Convert to host-endiannes */
    header.transactionIdentifier = ntohs(header.transactionIdentifier);
    header.protocolIdentifier = ntohs(header.protocolIdentifier);
    header.length = ntohs(header.length);

    /** 1. Verify protocol ID */
    if (header.protocolIdentifier != eModbus_protocolIdentifier) {
        return eError_invalid;
    }

    *pHeader = header;

    return eError_ok;
}
