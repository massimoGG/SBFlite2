#ifndef MODBUS_MBAP_H__
#define MODBUS_MBAP_H__

#include <error_codes.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
    /** @brief The length of an MBAP header */
    eModbus_mbapLength = 7U,

    /** @brief The MB protocol identifier */
    eModbus_protocolIdentifier = 0,

    /** @brief The length of an MBAP header */
    eModbusMbapHeaderLength = 7U,
};

/** @brief The MBAP Header */
typedef struct __attribute__((packed)) modbusMbapHeader {
    /** Transaction Identifier = identification of request/response */
    uint16_t transactionIdentifier;

    /** Protocol Identifier = 0 for MODBUS protocol */
    uint16_t protocolIdentifier;

    /** Length = number of following bytes including the Unit Identifier and Data Fields */
    uint16_t length;

    /** Unit Identifier = identification of a remote slave connected, should be 0xFF on TCP/IP */
    uint8_t unitIdentifier;
} modbusMbapHeader_t;

int modbus_encodeMbap(uint8_t* pBuf, uint16_t transactionID, uint16_t pduLength, uint8_t unitIdentifier);
error_e modbus_decodeMbap(const uint8_t* pBuf, uint16_t len, modbusMbapHeader_t* pHeader);

#ifdef __cplusplus
}
#endif

#endif