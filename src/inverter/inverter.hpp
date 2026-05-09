#ifndef __INVERTER_HPP__
#define __INVERTER_HPP__

#include <string>

/** Inverter's state */
struct SmaInverter_t
{
    double DayYield;
    double TotalYield;

    std::string Ip;
    unsigned short Port;
    std::string Name;

    int FeedIntime; // 30543

    double Temperature;         // 30953
    double HeatsinkTemperature; // 34109
    int Condition;              // 30201
    int GridRelay;              // 30217

    double Udc1;
    double Idc1;
    double Pdc1;

    double Udc2;
    double Idc2;
    double Pdc2;

    double Uac1;
    double Iac1;
    double Pac1;

    double GridFreq;      // 30803
    double ReactivePower; // 30805
    double ApparentPower; // 30813
};

std::string to_string(const SmaInverter_t &inv);

#endif