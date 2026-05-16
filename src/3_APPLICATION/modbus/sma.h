#ifndef MODBUS_WRAPPER_H__
#define MODBUS_WRAPPER_H__

#include <1_LL/network/network.h>
#include <2_DRIVERS/modbus/modbus.h>
#include <error_codes.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Handle for state */
typedef struct modbusWrapper_handle {

    /** @brief The Unit Identifier */
    unsigned unitIdentifier;

    /** @brief The network handle for this inverter */
    networkHandle_t* pNetworkHandle;

    /** @brief Buffer for response */
    uint8_t responseBuffer[eModbus_maximumPduSizeTcp];

    /** @brief The size of the buffer response */
    uint16_t responseSize;

} modbusWrapper_handle_t;

error_e sendReadHoldingRegister(modbusWrapper_handle_t* pHandle, uint16_t startingAddress, uint16_t count);
error_e fetchReadHoldingRegister(modbusWrapper_handle_t* pHandle, uint8_t count, modbusPduResponseReadHoldingRegistersHeader_t* pHoldingHeader);

error_e fetchU32(modbusWrapper_handle_t* pHandle, uint16_t address, uint32_t* pValue);
error_e fetchU32Multiple(modbusWrapper_handle_t* pHandle, uint16_t address, uint16_t count, uint32_t* pRegisters);

void printBufferHex(const uint8_t* pBuf, uint32_t length);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
