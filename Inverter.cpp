#include "Inverter.h"
#include "types.h"

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>

int Inverter::process()
{
    int rc = logOn();
    if (rc != 0)
    {
        logOff();
        return rc;
    }
    std::cout << "Logon OK";

    if ((rc = getInverterData(Type_SoftwareVersion)) != 0)
        std::cerr << "getSoftwareVersion returned an error: " << rc << std::endl;

    if ((rc = getInverterData(Type_TypeLabel)) != 0)
        std::cerr << "getTypeLabel returned an error: " << rc << std::endl;

    /*
     * Show info about inverter
     */
    printf("SUSyID: %d - SN: %lu\n", SUSyID, Serial);
    printf("Device Name:      %s\n", DeviceName.c_str());
    printf("Device Class:     %s\n", DeviceClass.c_str());
    printf("Device Type:      %s\n", DeviceType.c_str());
    printf("Software Version: %s\n", SWVersion.c_str());
    printf("Serial number:    %lu\n", Serial);

    if ((rc = getInverterData(Type_MeteringGridMsTotW)) < 0)
        std::cerr << "getMeteringGridInfo returned an error: " << rc << std::endl;
    else {
        if (rc == E_OK) {
            printf("SUSyID: %d - SN: %lu\n", SUSyID, Serial);
            printf("Grid Power Out : %dW\n", MeteringGridMsTotWOut);
            printf("Grid Power In  : %dW\n", MeteringGridMsTotWIn);
        }
    }

    if ((rc = getInverterData(Type_DeviceStatus)) != 0)
        std::cerr << "getDeviceStatus returned an error: " << rc << std::endl;
    else {
        printf("SUSyID: %d - SN: %lu\n", SUSyID, Serial);
        printf("Device Temperature: ");
        printf("%3.1f\n", ((float)Temperature / 100));
        // TODO Fix temperature decimal reading
    }

    if (DevClass == SolarInverter) {
        if ((rc = getInverterData(Type_GridRelayStatus)) != 0)
            std::cerr << "getGridRelayStatus returned an error: " << rc << std::endl;
        else {
            printf("SUSyID: %d - SN: %lu\n", SUSyID, Serial);
            printf("GridRelay Status:      %d\n", GridRelayStatus);
        }
    }

    if ((rc = getInverterData(Type_EnergyProduction)) != 0)
        std::cerr << "getEnergyProduction returned an error: " << rc << std::endl;

    if ((rc = getInverterData(Type_OperationTime)) != 0)
        std::cerr << "getOperationTime returned an error: " << rc << std::endl;
    else
    {
        printf("SUSyID: %d - SN: %lu\n", SUSyID, Serial);
        puts("Energy Production:");
        printf("\tEToday: %.3fkWh\n", tokWh(EToday));
        printf("\tETotal: %.3fkWh\n", tokWh(ETotal));
        printf("\tOperation Time: %.2fh\n", toHour(OperationTime));
        printf("\tFeed-In Time  : %.2fh\n", toHour(FeedInTime));
    }

    if ((rc = getInverterData(Type_SpotDCPower)) != 0)
        std::cerr << "getSpotDCPower returned an error: " << rc << std::endl;

    if ((rc = getInverterData(Type_SpotDCVoltage)) != 0)
        std::cerr << "getSpotDCVoltage returned an error: " << rc << std::endl;

    if ((rc = getInverterData(Type_SpotACPower)) != 0)
        std::cerr << "getSpotACPower returned an error: " << rc << std::endl;

    if ((rc = getInverterData(Type_SpotACVoltage)) != 0)
        std::cerr << "getSpotACVoltage returned an error: " << rc << std::endl;

    if ((rc = getInverterData(Type_SpotACTotalPower)) != 0)
        std::cerr << "getSpotACTotalPower returned an error: " << rc << std::endl;

    calPacTot = Pac1 + Pac2 + Pac3;
    calEfficiency = calPdcTot == 0 ? 0.0f : 100.0f * (float)calPacTot / (float)calPdcTot;

    puts("AC Spot Data:");
    printf("\tPhase 1 Pac : %7.3fkW - Uac: %6.2fV - Iac: %6.3fA\n", tokW(Pac1), toVolt(Uac1), toAmp(Iac1));
    printf("\tPhase 2 Pac : %7.3fkW - Uac: %6.2fV - Iac: %6.3fA\n", tokW(Pac2), toVolt(Uac2), toAmp(Iac2));
    printf("\tPhase 3 Pac : %7.3fkW - Uac: %6.2fV - Iac: %6.3fA\n", tokW(Pac3), toVolt(Uac3), toAmp(Iac3));
    printf("\tTotal Pac   : %7.3fkW - Calculated Pac: %7.3fkW\n", tokW(TotalPac), tokW(calPacTot));
    printf("\tEfficiency  : %7.2f%%\n", calEfficiency);

    if ((rc = getInverterData(Type_SpotGridFrequency)) != 0)
        std::cerr << "getSpotGridFrequency returned an error: " << rc << std::endl;
    else
    {
        printf("SUSyID: %d - SN: %lu\n", SUSyID, Serial);
        printf("Grid Freq. : %.2fHz\n", toHz(GridFreq));
    }

    if (DevClass == SolarInverter)
    {
        printf("SUSyID: %d - SN: %lu\n", SUSyID, Serial);
        if (InverterDatetime > 0)
            printf("Current Inverter Time: %s\n", strftime_t(InverterDatetime));

        if (WakeupTime > 0)
                    printf("Inverter Wake-Up Time: %s\n", strftime_t(WakeupTime));

        if (SleepTime > 0)
                    printf("Inverter Sleep Time  : %s\n", strftime_t(SleepTime));
    }

    logOff();

    return rc;
}

int Inverter::logOn()
{
    int rc = 0;

    printf("Connecting to Local Network...\n");

    rc = ethConnect(m_config.IP_Port);
    if (rc != 0)
    {
            print_error(stdout, PROC_CRITICAL, "Failed to set up socket connection.");
            return rc;
    }

    if (m_config.ip_addresslist.size() > 1)
        // New method for multiple inverters with fixed IP
        rc = ethInitConnectionMulti(m_config.ip_addresslist);
    else
        // Old method for one inverter (fixed IP or broadcast)
        rc = ethInitConnection(m_config.IP_Address);

    if (rc != E_OK)
    {
        print_error(stdout, PROC_CRITICAL, "Failed to initialize Speedwire connection.");
        ethClose();
        return rc;
    }

    rc = logonSMAInverter(m_config.userGroup, m_config.SMA_Password);
    if (rc != E_OK)
    {
        if (rc == E_INVPASSW)
            snprintf(msg, sizeof(msg), "Logon failed. Check '%s' Password\n", m_config.userGroup == UG_USER? "USER":"INSTALLER");
        else
            snprintf(msg, sizeof(msg), "Logon failed. Reason unknown (%d)\n", rc);

        print_error(stdout, PROC_CRITICAL, msg);
        bthClose();
        return 1;
    }

    /*************************************************
     * At this point we are logged on to the inverter
     *************************************************/

    return rc;
}

void Inverter::logOff()
{
}

int getInverterData(enum getInverterDataType type)
{

}

char *strftime_t (const time_t rawtime)
{
    static char buffer[256];
    struct tm tm_struct;
    memcpy(&tm_struct, localtime(&rawtime), sizeof(tm_struct));
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %X", &tm_struct);
    return buffer;
}
