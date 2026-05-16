/**
 * @file inverter.cpp
 * @brief implemnetation of an SMA inverter
 */

#include "inverter.hpp"

/**
 * @brief forms a printable inverter
 * @param inv
 * @returns string representation of given inverter
 */
std::string to_string(const SmaInverter_t& inv)
{
    using namespace std;

    string ret {};

    ret += "\n\n\n\033[1m---------------------------\nINVERTER - " + string(inv.name) + "\n---------------------------\033[0m\n";

    ret += "Total yield: " + to_string(inv.totalYield) + "Wh\n";

    ret += "Day yield: " + to_string(inv.dayYield) + "Wh\n";

    ret += "Inverter\n\tTemperature: " + to_string(inv.temperature) + "C\tHeatsink: " + to_string(inv.heatsinkTemperature) + "C\n";

    ret += "DC 1\n\tVolt: " + to_string(inv.udc1) + "V\n\tAmp: " + to_string(inv.idc1) + "A\n\tWatt: " + to_string(inv.pdc1) + "W\n";
    ret += "DC 2\n\tVolt: " + to_string(inv.udc2) + "V\n\tAmp: " + to_string(inv.idc2) + "A\n\tWatt: " + to_string(inv.pdc2) + "W\n";
    ret += "AC  \n\tVolt: " + to_string(inv.uac1) + "V\n\tAmp: " + to_string(inv.iac1) + "A\n\tWatt: " + to_string(inv.pac1) + "W\n";

    ret += "\tGridFreq: " + to_string(inv.gridFreq) + "\n\tReactiveP: " + to_string(inv.reactivePower) + " VAr\n\tApparentP: " + to_string(inv.apparentPower) + "\n";

    return ret;
}