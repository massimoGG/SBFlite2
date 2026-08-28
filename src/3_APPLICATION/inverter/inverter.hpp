#ifndef INVERTER_HPP__
#define INVERTER_HPP__

#include <1_LL/network/network.h>
#include <3_APPLICATION/modbus/modbus_sma.h>
#include <string>

/** Inverter's state */
struct SmaInverter_t {

    /** The connection to the inverter */
    networkHandle_t* networkHandle;

    uint8_t unitIdentifier;

    std::string name;

    /** PV system utility grid connection */
    smaModbus_pvSystemUtilityGridConnection_e gridConnection;

    /** Status of the device */
    smaModbus_statusOfTheDevice_e statusOfDevice;

    /** Total AC energy fed in on all line conductors (Wh) */
    FIX0 totalYield;

    /* Energy fed in on the current day on all line conductors (Wh) */
    FIX0 dayYield;

    /** Feed-in time (s) */
    Duration feedIntime;

    /** DC current input 1 (A) */
    FIX3 udc1;
    /** DC voltage input 1 (V) */
    FIX2 idc1;
    /** DC power input 1 (W) */
    FIX0 pdc1;

    /** Line voltage, line conductor L1 to N (V) */
    FIX2 uac1;
    /** Active power of line conductor L1 (W) */
    FIX0 pac1;
    /** Line current of line conductor L1 (A) */
    FIX3 iac1;

    /** Power frequency (Hz)*/
    FIX2 gridFreq;
    /** Reactive power on all line conductors (VAr) */
    FIX0 reactivePower;
    /** Apparent power on all line conductors (VA) */
    FIX0 apparentPower;

    /** Internal temperature (C) S32; TEMP; 30953 */
    TEMP temperature;

    /** DC current input 2 (A); S32; FIX3; 30957 */
    FIX3 idc2;
    /** DC voltage input 2 (V); S32; FIX2; 30959 */
    FIX2 udc2;
    /** DC power input 2 (W); S32; FIX0; 30961 */
    FIX0 pdc2;

    /** Heat sink temperature (C) S32; TEMP; 34109 */
    TEMP heatsinkTemperature;
};

std::string to_string(const SmaInverter_t& inv);

#endif