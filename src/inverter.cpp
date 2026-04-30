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
std::string to_string(const SMA_Inverter &inv)
{
    using namespace std;

    string ret{};

    ret += "\n\n\n\033[1m---------------------------\nINVERTER - " + string(inv.Name) + "\n" + string(inv.Ip) + "%s\n---------------------------\033[0m\n";

    ret += "Total yield: " + to_string(inv.TotalYield) + "Wh\n";

    ret += "Day yield: " + to_string(inv.DayYield) + "Wh\n";

    ret += "Inverter\n\tTemperature: " + to_string(inv.Temperature) + "C\tHeatsink: " + to_string(inv.HeatsinkTemperature) + "C\n";

    ret += "DC 1\n\tVolt: " + to_string(inv.Udc1) + "V\n\tAmp: " + to_string(inv.Idc1) + "A\n\tWatt: " + to_string(inv.Pdc1) + "W\n";
    ret += "DC 2\n\tVolt: " + to_string(inv.Udc2) + "V\n\tAmp: " + to_string(inv.Idc2) + "A\n\tWatt: " + to_string(inv.Pdc2) + "W\n";
    ret += "AC  \n\tVolt: " + to_string(inv.Uac1) + "V\n\tAmp: " + to_string(inv.Iac1) + "A\n\tWatt: " + to_string(inv.Pac1) + "W\n";

    ret += "\tGridFreq: " + to_string(inv.GridFreq) + "\n\tReactiveP: " + to_string(inv.ReactivePower) + " VAr\n\tApparentP: " + to_string(inv.ApparentPower) + "\n";

    return ret;
}