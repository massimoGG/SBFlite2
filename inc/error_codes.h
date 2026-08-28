/**
 * @file error_codes.h
 * @brief
 */
#ifndef __ERROR_CODES_H__
#define __ERROR_CODES_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @brief error flags */
typedef enum error {
    eError_ok = 0, /**< No error  */
    eError_failed = 0x01, /**< Generic error */
    eError_busy = 0x02, /**< Busy */
    eError_invalid = 0x04, /**< Invalid state/argument */
    eError_invalidLength = 0x08, /**< Invalid supplied length */
    eError_invalidData = 0x10, /**< Invalid supplied data */
    eError_timeout = 0x20, /**< Timeout */
    eError_unsupported = 0x40, /**< Unsupported */
    eError_noData = 0x80, /**< No data */
} error_e;

#ifdef __cplusplus
}
#endif

#endif