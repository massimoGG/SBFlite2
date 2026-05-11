/**
 * @file error_codes.h
 * @brief
 */
#ifndef __ERROR_CODES_H__
#define __ERROR_CODES_H__

#ifdef __cplusplus__
extern "C" {
#endif

/** @brief error flags */
typedef enum error {
    eError_ok = 0, /**< No error  */
    eError_failed, /**< Generic error */
    eError_busy, /**< Busy */
    eError_invalid, /**< Invalid state/argument */
    eError_invalidLength, /**< Invalid supplied length */
    eError_invalidData, /**< Invalid supplied data */
    eError_timeout /**< Timeout */
} error_e;

#ifdef __cplusplus__
}
#endif

#endif