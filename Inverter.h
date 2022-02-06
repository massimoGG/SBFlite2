#ifndef __HEADER_INVERTER
#define __HEADER_INVERTER

#include "types.h"

extern unsigned int cmdcode;
extern int packetposition;
extern int FCSChecksum;
extern unsigned short pcktID;

extern unsigned char pcktBuf[];

extern unsigned char  RootDeviceAddress[6];
extern unsigned char  LocalBTAddress[6];
extern unsigned char  addr_broadcast[6];
extern unsigned char  addr_unknown[6];
extern unsigned short AppSUSyID;
extern unsigned long  AppSerial;
extern const unsigned short anySUSyID;
extern const unsigned long anySerial;
#define ETH_L2SIGNATURE 0x65601000

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
    unsigned long BatChaStt;			// Current battery charge status
    unsigned long BatDiagCapacThrpCnt;	// Number of battery charge throughputs
    unsigned long BatDiagTotAhIn;		// Amp hours counter for battery charge
    unsigned long BatDiagTotAhOut;		// Amp hours counter for battery discharge
    unsigned long BatTmpVal;			// Battery temperature
    unsigned long BatVol;				// Battery voltage
    long BatAmp;						// Battery current
    int32_t Temperature;                // Inverter Temperature
    int32_t	MeteringGridMsTotWOut;		// Power grid feed-in (Out)
    int32_t MeteringGridMsTotWIn;		// Power grid reference (In)
    int logonStatus;
};

#endif
