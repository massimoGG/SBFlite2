#ifndef MODBUS_H__
#define MODBUS_H__

#include "modbus_mbap.h"
#include "modbus_pdu.h"
#include <error_codes.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
    /** @brief The MB port */
    eModbus_port = 502,

    /** @brief The maximum size of a whole PDU packet */
    eModbus_maximumPduSizeTcp = eModbus_maximumPduSize + eModbus_mbapLength,
};

/** @brief Handle for MODBUS functions, keeping the state of the transaction ID */
typedef struct modbusHandle {

    /** The transaction ID expected to receive from the server */
    uint16_t lastSentTransactionIdentifier;

} modbusHandle_t;

/** @brief Function pointer to write to the network layer */
typedef error_e (*fWrite)(const uint8_t* pData, uint16_t len);

/** @brief Function pointer to read from the network layer */
typedef error_e (*fRead)(uint8_t* pData, uint16_t len);

/** @brief Function pointer for exception handler */
typedef void (*fExceptionHandler)(uint16_t transactionNumber, modbusPduExceptionResponseHeader_t exceptionHeader);

/** @brief Function pointer upon a decoded register PDU */
typedef void (*fPduRegisterCb)(uint16_t transactionNumber, uint16_t registerValue);

/** @brief collection of callbacks for each PDU response type */
typedef struct modbusDecodeCbs {

    /** @brief Exception callback */
    fExceptionHandler exceptionCallback;

    /** @brief Read holding register callback */
    fPduRegisterCb readHoldingRegisterCallback;
} modbusDecodeCbs_t;

/** Public Prototypes */

error_e modbus_decode(const uint8_t* pBuf, uint16_t len, modbusDecodeCbs_t* pCallbacks);

#ifdef __cplusplus
} // extern "C"
#endif

#endif