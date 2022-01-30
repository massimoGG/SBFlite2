#pragma once

#include <string>

#define tokWh(value64) (double)(value64)/1000
#define tokW(value32) (float)(value32)/1000
#define toHour(value64) (double)(value64)/3600
#define toAmp(value32) (float)value32/1000
#define toVolt(value32) (float)value32/100
#define toHz(value32) (float)value32/100
#define toTemp(value32) (float)value32/100

struct MonthData
{
    time_t datetime;
    long long totalWh;	// changed to signed - issue 58
    long long dayWh;	// changed to signed - issue 58
};

struct DayData
{
    time_t datetime;
    long long totalWh;	// changed to signed - issue 58
    long long watt;		// changed to signed - issue 58
};

enum DEVICECLASS
{
    AllDevices = 8000,          // DevClss0
    SolarInverter = 8001,       // DevClss1
    WindTurbineInverter = 8002, // DevClss2
    BatteryInverter = 8007,     // DevClss7
    Consumer = 8033,            // DevClss33
    SensorSystem = 8064,        // DevClss64
    ElectricityMeter = 8065,    // DevClss65
    CommunicationProduct = 8128 // DevClss128
};

enum getInverterDataType
{
    Type_EnergyProduction	= 1 << 0,
    Type_SpotDCPower			= 1 << 1,
    Type_SpotDCVoltage		= 1 << 2,
    Type_SpotACPower			= 1 << 3,
    Type_SpotACVoltage		= 1 << 4,
    Type_SpotGridFrequency	= 1 << 5,
    //Type_MaxACPower			= 1 << 6,
    //Type_MaxACPower2			= 1 << 7,
    Type_SpotACTotalPower	= 1 << 8,
    Type_TypeLabel			= 1 << 9,
    Type_OperationTime		= 1 << 10,
    Type_SoftwareVersion		= 1 << 11,
    Type_DeviceStatus		= 1 << 12,
    Type_GridRelayStatus		= 1 << 13,
    Type_BatteryChargeStatus = 1 << 14,
    Type_BatteryInfo         = 1 << 15,
    Type_InverterTemperature	= 1 << 16,
    Type_MeteringGridMsTotW	= 1 << 17,

    Type_sbftest             = 1 << 31
};

enum SMA_DATATYPE
{
    DT_ULONG = 0,
    DT_STATUS = 8,
    DT_STRING = 16,
    DT_FLOAT = 32,
    DT_SLONG = 64
};

//SMA Structs must be aligned on byte boundaries
#pragma pack(push, 1)
typedef struct PacketHeader
{
    unsigned char  SOP;					// Start Of Packet (0x7E)
    unsigned short pkLength;
    unsigned char  pkChecksum;
    unsigned char  SourceAddr[6];		// SMA Inverter Address
    unsigned char  DestinationAddr[6];	// Local BT Address
    unsigned short command;
} pkHeader;

struct ethPacketHeaderL1
{
    uint32_t      MagicNumber;      // Packet signature 53 4d 41 00 (SMA\0)
    uint32_t      unknown1;         // 00 04 02 a0
    uint32_t      unknown2;         // 00 00 00 01
    unsigned char hiPacketLen;      // Packet length stored as big endian
    unsigned char loPacketLen ;     // Packet length Low Byte
};

struct ethPacketHeaderL2
{
    uint32_t      MagicNumber;      // Level 2 packet signature 00 10 60 65
    unsigned char longWords;        // int(PacketLen/4)
    unsigned char ctrl;
};

struct ethPacketHeaderL1L2
{
    ethPacketHeaderL1 pcktHdrL1;
    ethPacketHeaderL2 pcktHdrL2;
};

struct ethEndpoint
{
    unsigned short SUSyID;
    uint32_t       Serial;
    unsigned short Ctrl;
};

struct ethPacket
{
    unsigned char dummy0;
    ethPacketHeaderL2 pcktHdrL2;
    ethEndpoint Destination;
    ethEndpoint Source;
    unsigned short ErrorCode;
    unsigned short FragmentID;  //Count Down
    unsigned short PacketID;    //Count Up
};

struct ArchiveDataRec
{
    time_t datetime;
    unsigned long long totalWh;
};
#pragma pack(pop)

typedef unsigned char BYTE;

enum LriDef
{
    OperationHealth                 = 0x00214800,   // *08* Condition (aka INV_STATUS)
    CoolsysTmpNom                   = 0x00237700,   // *40* Operating condition temperatures
    DcMsWatt                        = 0x00251E00,   // *40* DC power input (aka SPOT_PDC1 / SPOT_PDC2)
    MeteringTotWhOut                = 0x00260100,   // *00* Total yield (aka SPOT_ETOTAL)
    MeteringDyWhOut                 = 0x00262200,   // *00* Day yield (aka SPOT_ETODAY)
    GridMsTotW                      = 0x00263F00,   // *40* Power (aka SPOT_PACTOT)
    BatChaStt                       = 0x00295A00,   // *00* Current battery charge status
    OperationHealthSttOk            = 0x00411E00,   // *00* Nominal power in Ok Mode (deprecated INV_PACMAX1)
    OperationHealthSttWrn           = 0x00411F00,   // *00* Nominal power in Warning Mode (deprecated INV_PACMAX2)
    OperationHealthSttAlm           = 0x00412000,   // *00* Nominal power in Fault Mode (deprecated INV_PACMAX3)
    OperationGriSwStt               = 0x00416400,   // *08* Grid relay/contactor (aka INV_GRIDRELAY)
    OperationRmgTms                 = 0x00416600,   // *00* Waiting time until feed-in
    DcMsVol                         = 0x00451F00,   // *40* DC voltage input (aka SPOT_UDC1 / SPOT_UDC2)
    DcMsAmp                         = 0x00452100,   // *40* DC current input (aka SPOT_IDC1 / SPOT_IDC2)
    MeteringPvMsTotWhOut            = 0x00462300,   // *00* PV generation counter reading
    MeteringGridMsTotWhOut          = 0x00462400,   // *00* Grid feed-in counter reading
    MeteringGridMsTotWhIn           = 0x00462500,   // *00* Grid reference counter reading
    MeteringCsmpTotWhIn             = 0x00462600,   // *00* Meter reading consumption meter
    MeteringGridMsDyWhOut           = 0x00462700,   // *00* ?
    MeteringGridMsDyWhIn            = 0x00462800,   // *00* ?
    MeteringTotOpTms                = 0x00462E00,   // *00* Operating time (aka SPOT_OPERTM)
    MeteringTotFeedTms              = 0x00462F00,   // *00* Feed-in time (aka SPOT_FEEDTM)
    MeteringGriFailTms              = 0x00463100,   // *00* Power outage
    MeteringWhIn                    = 0x00463A00,   // *00* Absorbed energy
    MeteringWhOut                   = 0x00463B00,   // *00* Released energy
    MeteringPvMsTotWOut             = 0x00463500,   // *40* PV power generated
    MeteringGridMsTotWOut           = 0x00463600,   // *40* Power grid feed-in
    MeteringGridMsTotWIn            = 0x00463700,   // *40* Power grid reference
    MeteringCsmpTotWIn              = 0x00463900,   // *40* Consumer power
    GridMsWphsA                     = 0x00464000,   // *40* Power L1 (aka SPOT_PAC1)
    GridMsWphsB                     = 0x00464100,   // *40* Power L2 (aka SPOT_PAC2)
    GridMsWphsC                     = 0x00464200,   // *40* Power L3 (aka SPOT_PAC3)
    GridMsPhVphsA                   = 0x00464800,   // *00* Grid voltage phase L1 (aka SPOT_UAC1)
    GridMsPhVphsB                   = 0x00464900,   // *00* Grid voltage phase L2 (aka SPOT_UAC2)
    GridMsPhVphsC                   = 0x00464A00,   // *00* Grid voltage phase L3 (aka SPOT_UAC3)
    GridMsAphsA_1                   = 0x00465000,   // *00* Grid current phase L1 (aka SPOT_IAC1)
    GridMsAphsB_1                   = 0x00465100,   // *00* Grid current phase L2 (aka SPOT_IAC2)
    GridMsAphsC_1                   = 0x00465200,   // *00* Grid current phase L3 (aka SPOT_IAC3)
    GridMsAphsA                     = 0x00465300,   // *00* Grid current phase L1 (aka SPOT_IAC1_2)
    GridMsAphsB                     = 0x00465400,   // *00* Grid current phase L2 (aka SPOT_IAC2_2)
    GridMsAphsC                     = 0x00465500,   // *00* Grid current phase L3 (aka SPOT_IAC3_2)
    GridMsHz                        = 0x00465700,   // *00* Grid frequency (aka SPOT_FREQ)
    MeteringSelfCsmpSelfCsmpWh      = 0x0046AA00,   // *00* Energy consumed internally
    MeteringSelfCsmpActlSelfCsmp    = 0x0046AB00,   // *00* Current self-consumption
    MeteringSelfCsmpSelfCsmpInc     = 0x0046AC00,   // *00* Current rise in self-consumption
    MeteringSelfCsmpAbsSelfCsmpInc  = 0x0046AD00,   // *00* Rise in self-consumption
    MeteringSelfCsmpDySelfCsmpInc   = 0x0046AE00,   // *00* Rise in self-consumption today
    BatDiagCapacThrpCnt             = 0x00491E00,   // *40* Number of battery charge throughputs
    BatDiagTotAhIn                  = 0x00492600,   // *00* Amp hours counter for battery charge
    BatDiagTotAhOut                 = 0x00492700,   // *00* Amp hours counter for battery discharge
    BatTmpVal                       = 0x00495B00,   // *40* Battery temperature
    BatVol                          = 0x00495C00,   // *40* Battery voltage
    BatAmp                          = 0x00495D00,   // *40* Battery current
    NameplateLocation               = 0x00821E00,   // *10* Device name (aka INV_NAME)
    NameplateMainModel              = 0x00821F00,   // *08* Device class (aka INV_CLASS)
    NameplateModel                  = 0x00822000,   // *08* Device type (aka INV_TYPE)
    NameplateAvalGrpUsr             = 0x00822100,   // *  * Unknown
    NameplatePkgRev                 = 0x00823400,   // *08* Software package (aka INV_SWVER)
    InverterWLim                    = 0x00832A00,   // *00* Maximum active power device (deprecated INV_PACMAX1_2) (Some inverters like SB3300/SB1200)
    GridMsPhVphsA2B6100             = 0x00464B00,
    GridMsPhVphsB2C6100             = 0x00464C00,
    GridMsPhVphsC2A6100             = 0x00464D00
};


enum E_SBFSPOT
{
    E_LRINOTAVAIL   =  21,  // Requested LRI not available
    E_OK            =  0,   // No error
    E_NODATA        = -1,   // Bluetooth buffer empty
    E_BADARG        = -2,   // Unknown command line argument
    E_CHKSUM        = -3,   // Invalid Checksum
    E_BUFOVRFLW     = -4,   // Buffer overflow
    E_ARCHNODATA    = -5,   // No archived data found for given timespan
    E_INIT          = -6,   // Unable to initialize
    E_INVPASSW      = -7,   // Invalid password
    E_RETRY         = -8,   // Retry the last action
    E_EOF           = -9,   // End of data
    E_PRIVILEGE     = -10,  // Privilege not held (need installer login)
    E_LOGONFAILED   = -11,  // Logon failed, other than Invalid Password (E_INVPASSW)
    E_COMM          = -12   // General communication error
};
