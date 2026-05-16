/**
 * @file modbus.c
 * @brief the modbus implementation
 */

#include "modbus.h"
#include <arpa/inet.h> // for htons and ntohs
#include <string.h>

/**
 * @brief decodes the given buffer
 * @param[in] pBuf          pointer to whole packet
 * @param[in] bufferLength  length of buffer
 * @param[in] pCallbacks    pointer to a collection of callbacks depending on the received PDU function code
 * @retval eError_ok        successfully decoded MB packet
 * @retval eError_failed    failed to decode
 * @retval eError_invalid   invalid data
 */
error_e
modbus_decode(const uint8_t* pBuf, uint16_t bufferLength, modbusDecodeCbs_t* pCallbacks)
{
    /* decode MBAP */
    modbusMbapHeader_t mbap = {};

    error_e ret = modbus_decodeMbap(pBuf, bufferLength, &mbap);
    if (eError_ok != ret) {
        return ret;
    }

    /* decode PDU */
    modbusPduResponseHeader_t pdu = {};

    ret = modbus_decodePdu(pBuf, bufferLength, &pdu);
    if (eError_ok != ret) {
        return eError_invalidData;
    }

    /* Depending on response code */
    if ((pdu.code & eModbusFunctionCode_exceptionOffset) != 0) {

        modbusPduExceptionResponseHeader_t exceptionheader = {};

        if (eError_ok != modbus_decodePduException(pBuf, bufferLength, &exceptionheader)) {
            return eError_invalidData;
        }

        pCallbacks->exceptionCallback(mbap.transactionIdentifier, exceptionheader);

    } else {

        switch (pdu.code) {
        case eModbusFunctionCode_readCoils:
            break;
        case eModbusFunctionCode_readDiscreteInputs:
            break;
        case eModbusFunctionCode_readHoldingRegister:
            /* Convert to */
            modbusPduResponseReadHoldingRegistersHeader_t holdingHeader = {};

            if (eError_ok != modbus_decodePduReadHoldingRegisterHeader(pBuf, bufferLength, &holdingHeader)) {
                return eError_invalidData;
            }

            uint8_t offset = 0;
            uint16_t registerValue = 0;
            while (eError_noData != modbus_getNextRegister(pBuf, bufferLength, &holdingHeader, &offset, &registerValue)) {
                pCallbacks->readHoldingRegisterCallback(mbap.transactionIdentifier, registerValue);
            }

            break;
        default:
            return eError_unsupported;
        }
    }

    return eError_ok;
}