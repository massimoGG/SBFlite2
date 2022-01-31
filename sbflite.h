#ifndef __sbflite_h_
#define __sbflite_h_

#include "Inverter.h"
#include "Ethernet.h"
#include "InfluxDB.h"
#include "types.h"

#include <iostream>

#define DEBUG_NORMAL 1

#define NaN_S16 0x8000                          // "Not a Number" representation for int16_t
#define NaN_U16 0xFFFF                          // "Not a Number" representation for uint16_t
#define NaN_S32 (int32_t) 0x80000000            // "Not a Number" representation for int32_t
#define NaN_U32 (uint32_t)0xFFFFFFFF            // "Not a Number" representation for uint32_t
#define NaN_S64 (int64_t) 0x8000000000000000    // "Not a Number" representation for int64_t
#define NaN_U64 (uint64_t)0xFFFFFFFFFFFFFFFF    // "Not a Number" representation for uint64_t

inline const bool is_NaN(const int16_t S16)
{
    return S16 == NaN_S16;
}

inline const bool is_NaN(const uint16_t U16)
{
    return U16 == NaN_U16;
}

inline const bool is_NaN(const int32_t S32)
{
    return S32 == NaN_S32;
}

inline const bool is_NaN(const uint32_t U32)
{
    return U32 == NaN_U32;
}

inline const bool is_NaN(const int64_t S64)
{
    return S64 == NaN_S64;
}

inline const bool is_NaN(const uint64_t U64)
{
    return U64 == NaN_U64;
}

#endif
