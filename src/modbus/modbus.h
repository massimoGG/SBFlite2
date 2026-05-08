#ifndef MODBUS_H__
#define MODBUS_H__

#include <error_codes.h>
#include <stdint.h>

#ifdef __cplusplus__
extern "C" {
#endif

/** @brief MODBUS Function Codes */
typedef enum modbusFunctionCodes {
    eFunctionCode_readCoils = 0x01,
    eFunctionCode_readDiscreteInputs = 0x02,
    eFunctionCode_readHoldingRegister = 0x03,
    eFunctionCode_readInputRegister = 0x04,
    eFunctionCode_writeSingleCoil = 0x05,
    eFunctionCode_writeSingleRegister = 0x06,
    eFunctionCode_readExceptionStatus = 0x07,
    eFunctionCode_diagnostics = 0x08,
    eFunctionCode_getCommEventCounter = 0x0B,
    eFunctionCode_getCommEventLog = 0x0C,
    eFunctionCode_writeMultipleCoils = 0x0F,
    eFunctionCode_writeMultipleRegisters = 0x10,
    eFunctionCode_reportServerId = 0x11,
    eFunctionCode_readFileRecord = 0x14,
    eFunctionCode_writeFileRecord = 0x015,
    eFunctionCode_maskWriteRegister = 0x16,
    eFunctionCode_readWriteMultipleRegisters = 0x17,
    eFunctionCode_readFifoQueue = 0x18,
    eFunctionCode_encapsulatedInterfaceTransport = 0x2B,
} modbusFunctionCodes_e;

typedef error_e (*fWrite)(const uint8_t* pData, uint16_t len);
typedef error_e (*fRead)(uint8_t* pData, uint16_t len);

error_e modbus_init(fRead, fWrite);

void modbus_connect(void);
bool modbus_getState(void);
float modbus_readFloat32(void);
void modbus_disconnect(void);

#ifdef __cplusplus__
} // extern "C"
#endif

#endif