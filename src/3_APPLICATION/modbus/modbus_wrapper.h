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

error_e sendRequest(modbusWrapper_handle_t* pHandle, uint16_t startingAddress, uint16_t count);
error_e readResponse(modbusWrapper_handle_t* pHandle, uint8_t count);

void printBufferHex(const uint8_t* pBuf, uint32_t length);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
