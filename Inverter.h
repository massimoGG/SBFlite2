#ifndef __inverter_h_
#define __inverter_h_

#include "sbflite.h"
#include "types.h"
#include "Ethernet.h"

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Wellknown SUSyID's
#define SID_MULTIGATE	175
#define SID_SB240		244

char *strftime_t (const time_t rawtime);

class Inverter
{
public:
    Inverter(std::string IPAddress) {
        this->IPAddress = IPAddress;
    }
    ~Inverter() {
        logOff();
    }
    int process();

private:
    int logOn();
    void logOff();

    int getInverterData(enum getInverterDataType type);
    E_SBFSPOT getPacket(unsigned char senderaddr[6], int wait4Command);
    E_SBFSPOT logonSMAInverter(long userGroup, const char *password);
    E_SBFSPOT logoffSMAInverter();

    std::string DeviceName;
    std::string IPAddress;
    unsigned short SUSyID;
    unsigned long Serial;
    unsigned char NetID;
    time_t InverterDatetime;
    time_t WakeupTime;
    time_t SleepTime;
    long Pdc1;
    long Pdc2;
    long Udc1;
    long Udc2;
    long Idc1;
    long Idc2;
    long Pmax1;
    long Pmax2;
    long Pmax3;
    long TotalPac;
    long Pac1;
    long Pac2;
    long Pac3;
    long Uac1;
    long Uac2;
    long Uac3;
    long Iac1;
    long Iac2;
    long Iac3;
    long GridFreq;
    long long OperationTime;
    long long FeedInTime;
    long long EToday;
    long long ETotal;
    unsigned short modelID;
    std::string DeviceType;
    std::string DeviceClass;
    DEVICECLASS DevClass;
    std::string SWVersion; // "03.01.05.R"
    int DeviceStatus;
    int GridRelayStatus;
    int flags;
    DayData dayData[288];
    bool hasDayData;
    MonthData monthData[31];
    bool hasMonthData;
    time_t monthDataOffset;	// Issue 115
    long calPdcTot;
    long calPacTot;
    float calEfficiency;
    int32_t Temperature;                // Inverter Temperature
    int32_t	MeteringGridMsTotWOut;		// Power grid feed-in (Out)
    int32_t MeteringGridMsTotWIn;		// Power grid reference (In)
    int logonStatus;
};

#endif
