#ifndef INVERTER_HPP__
#define INVERTER_HPP__

#include <2_DRIVERS/modbus/modbus_sma.h>
#include <string>

/** Inverter's state */
struct SmaInverter_t
{
    std::string Ip;
    unsigned short Port;
    std::string Name;

    /** Status of the device */
    smaModbus_statusOfTheDevice_e Condition;

    /** Utility grid contactor */
    smaModbus_utilityGridContactor_e GridRelay;

    /** Total AC energy fed in on all line conductors (Wh) */
    FIX0 TotalYield;

    /* Energy fed in on the current day on all line conductors (Wh) */
    FIX0 DayYield;

    /** Feed-in time (s) */
    Duration FeedIntime;

    /** DC current input 1 (A) */
    FIX3 Udc1;
    /** DC voltage input 1 (V) */
    FIX2 Idc1;
    /** DC power input 1 (W) */
    FIX0 Pdc1;

    /** Line voltage, line conductor L1 to N (V) */
    FIX2 Uac1;
    /** Active power of line conductor L1 (W) */
    FIX0 Pac1;
    /** Line current of line conductor L1 (A) */
    FIX3 Iac1;

    /** Power frequency (Hz)*/
    FIX2 GridFreq;
    /** Reactive power on all line conductors (VAr) */
    FIX0 ReactivePower;
    /** Apparent power on all line conductors (VA) */
    FIX0 ApparentPower;

    /** Internal temperature (C) S32; TEMP; 30953 */
    TEMP Temperature;

    /** DC current input 2 (A); S32; FIX3; 30957 */
    FIX3 Idc2;
    /** DC voltage input 2 (V); S32; FIX2; 30959 */
    FIX2 Udc2;
    /** DC power input 2 (W); S32; FIX0; 30961 */
    FIX0 Pdc2;

    /** Heat sink temperature (C) S32; TEMP; 34109 */
    TEMP HeatsinkTemperature;
};

std::string to_string(const SmaInverter_t &inv);

#endif