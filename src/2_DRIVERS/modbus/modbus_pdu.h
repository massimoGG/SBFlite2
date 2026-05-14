#include <error_codes.h>
#include <stdint.h>

#ifndef MODBUS_PDU_H__
#define MODBUS_PDU_H__

#ifdef __cplusplus
extern "C" {
#endif

enum {
    /** @brief The maximum size of a PDU */
    eModbus_maximumPduSize = 253U,

    /** @brief The length of a PDU request header */
    eModbusPduRequestHeaderLength = 5U,

    /** @brief The length of a generic PDU response header */
    eModbusPduResponseHeaderLength = 1U,

    /** @brief The length of a generic PDU response header */
    eModbusPduResponseExceptionHeaderLength = 2U,

    /** @brief The length of a read holding register header */
    eModbusPduResponseReadHoldingRegisterHeaderLength = 2U,
};

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
    eModbusFunctionCode_exceptionOffset = 0x80
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

/** @brief MB PDU generic response header */
typedef struct __attribute__((packed)) modbusPduResponseHeader {

    /** Function/Exception code */
    uint8_t code;

} modbusPduResponseHeader_t;

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

} modbusPduResponseReadHoldingRegistersHeader_t;

int modbus_encodePdu(uint8_t* pBuf, modbusFunctionCodes_e code, uint16_t startingAddress, uint16_t quantity);
error_e modbus_decodePdu(const uint8_t* pBuf, uint16_t len, modbusPduResponseHeader_t* pHeader);
modbusFunctionCodes_e modbus_getPduFunctionCode(modbusPduResponseHeader_t* pHeader);
error_e modbus_decodePduException(const uint8_t* pBuf, uint16_t length, modbusPduExceptionResponseHeader_t* pHeader);
error_e modbus_decodePduReadHoldingRegisterHeader(const uint8_t* pBuf, uint16_t length, modbusPduResponseReadHoldingRegistersHeader_t* pHeader);
error_e modbus_getNextRegister(const uint8_t* pBuf, uint16_t bufferLength, modbusPduResponseReadHoldingRegistersHeader_t* pHeader, uint8_t* pByteCountOffset, uint16_t* pValue);

#ifdef __cplusplus
}
#endif

#endif