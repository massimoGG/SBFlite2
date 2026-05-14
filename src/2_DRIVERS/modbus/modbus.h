#ifndef MODBUS_H__
#define MODBUS_H__

#include <error_codes.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
    eModbus_port = 502,
    eModbus_replyMaximumSizeTcp = 260U,
};

/** @brief Handle for MODBUS functions, keeping the state of the transaction ID */
typedef struct modbusHandle {

    /** The transaction ID expected to receive from the server */
    uint16_t lastSentTransactionIdentifier;

} modbusHandle_t;

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

/** @brief The length of an MBAP header */
const uint32_t c_modbusMbapHeaderLength = 7U;

/** @brief MODBUS Function Codes */
typedef enum modbusFunctionCodes {
    eModbusFunctionCode_readCoils = 0x01,
    eModbusFunctionCode_readDiscreteInputs = 0x02,
    eModbusFunctionCode_readHoldingRegister = 0x03,
    eModbusFunctionCode_readInputRegister = 0x04,
    eModbusFunctionCode_writeSingleCoil = 0x05,
    eModbusFunctionCode_writeSingleRegister = 0x06,
    eModbusFunctionCode_readExceptionStatus = 0x07,
    eModbusFunctionCode_diagnostics = 0x08,
    eModbusFunctionCode_getCommEventCounter = 0x0B,
    eModbusFunctionCode_getCommEventLog = 0x0C,
    eModbusFunctionCode_writeMultipleCoils = 0x0F,
    eModbusFunctionCode_writeMultipleRegisters = 0x10,
    eModbusFunctionCode_reportServerId = 0x11,
    eModbusFunctionCode_readFileRecord = 0x14,
    eModbusFunctionCode_writeFileRecord = 0x015,
    eModbusFunctionCode_maskWriteRegister = 0x16,
    eModbusFunctionCode_readWriteMultipleRegisters = 0x17,
    eModbusFunctionCode_readFifoQueue = 0x18,
    eModbusFunctionCode_encapsulatedInterfaceTransport = 0x2B,
} modbusFunctionCodes_e;

/** @brief MB PDU generic request header (applicable to 01, 02, 03, 04, 05, ..) */
typedef struct __attribute__((packed)) modbusPduRequestHeader {

    /** Function Code */
    uint8_t functionCode;

    /** Starting Address */
    uint16_t startingAddress;

    /** Quantity */
    uint16_t quantity;

} modbusPduRequestHeader_t;

/** @brief The length of a PDU request header */
const uint32_t c_modbusPduRequestHeaderLength = 5U;

/** @brief MB PDU Exception response header */
typedef struct __attribute__((packed)) modbusPduExceptionResponseHeader {

    /** Function Code  */
    uint8_t functionCode;

    /** Exception Code */
    uint8_t exceptionCode;

} modbusPduExceptionResponseHeader_t;

/** @brief MB PDU Read Coils (01) response header */
typedef struct __attribute__((packed)) modbusPduReadCoilsResponseHeader {

    /** Function Code  */
    uint8_t functionCode;

    /** Byte count */
    uint8_t byteCount;

} modbusPduReadCoilsResponseHeader_t;

/** @brief MB PSU Read Holding Registers (03) response header */
typedef struct __attribute__((packed)) modbusPduReadHoldingRegistersResponseHeader {

    /** Function Code  */
    uint8_t functionCode;

    /** Byte count */
    uint8_t byteCount;

} modbusPduReadHoldingRegistersResponseHeader_t;

/** Function pointer to write to the network layer */
typedef error_e (*fWrite)(const uint8_t* pData, uint16_t len);

/** Function pointer to read from the network layer */
typedef error_e (*fRead)(uint8_t* pData, uint16_t len);

/** Public Prototypes */

int modbus_encodePdu(uint8_t* pBuf, modbusFunctionCodes_e code, uint16_t startingAddress, uint16_t quantity);
int modbus_encodeMbap(uint8_t* pBuf, uint16_t transactionID, uint16_t pduLength);

#ifdef __cplusplus
} // extern "C"
#endif

#endif