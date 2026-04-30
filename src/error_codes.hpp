/**
 * @file error_codes.hpp
 * @brief
 */
#ifndef __ERROR_CODES_HPP_
#define __ERROR_CODES_HPP_

/** @brief error flags */
enum error_e
{
    eError_ok = 0,        /**< No error  */
    eError_failed,        /**< Generic error */
    eError_busy,          /**< Busy */
    eError_invalid,       /**< Invalid state/argument */
    eError_invalidLength, /**< Invalid supplied length */
    eError_invalidData,   /**< Invalid supplied data */
};

#endif