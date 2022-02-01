#include "Inverter.h"


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

    // Calculate missing spot data
    if (Pdc1 == 0) Pdc1 = (Idc1 * Udc1) / 100000;
    if (Pdc2 == 0) Pdc2 = (Idc2 * Udc2) / 100000;
    if (Pac1 == 0) Pac1 = (Iac1 * Uac1) / 100000;
    if (Pac2 == 0) Pac2 = (Iac2 * Uac2) / 100000;
    if (Pac3 == 0) Pac3 = (Iac3 * Uac3) / 100000;
    if (TotalPac == 0) TotalPac = Pac1 + Pac2 + Pac3;

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

/**  TODO MOVE THE SOCKET SETUP OUT OF INVERTER!
 *  A socket shouldn't be created for each inverter!
 * Also fix bluetooth library
 */
int Inverter::logOn()
{
    int rc = E_OK;
    char msg[80];

    // TODO Initialize Speedwire connection to inverter
    AppSUSyID = 125;
    srand(time(NULL));
    AppSerial = 900000000 + ((rand() << 16) + rand()) % 100000000;
    printf("SUSyID: %d - SessionID: %lu\n", AppSUSyID, AppSerial);

    writePacketHeader(pcktBuf, 0, NULL);
    writePacket(pcktBuf, 0x09, 0xA0, 0, anySUSyID, anySerial);
    writeLong(pcktBuf, 0x00000200);
    writeLong(pcktBuf, 0);
    writeLong(pcktBuf, 0);
    writeLong(pcktBuf, 0);
    writePacketLength(pcktBuf);

    ethSend(pcktBuf, IPAddress.c_str());

    if ((rc = ethGetPacket()) == E_OK) {

        ethPacket *pckt = (ethPacket *)pcktBuf;
        SUSyID = btohs(pckt->Source.SUSyID);
        Serial = btohl(pckt->Source.Serial);
        printf("Inverter replied: %s SUSyID: %d - Serial: %lu\n", IPAddress.c_str(), SUSyID, Serial);

        logoffSMAInverter();
    } else {
        std::cerr << "ERROR: Speedwire Connection to inverter failed!" << std::endl;
        std::cerr << "Is " << IPAddress.c_str() << " the correct IP?" << std::endl;
        return rc;
    }

    int userGroup = UG_USER;
    rc = logonSMAInverter(userGroup, SMA_Password);
    if (rc != E_OK) {
        if (rc == E_INVPASSW)
            snprintf(msg, sizeof(msg), "Logon failed. Check '%s' Password\n", userGroup == UG_USER? "USER":"INSTALLER");
        else
            snprintf(msg, sizeof(msg), "Logon failed. Reason unknown (%d)\n", rc);

        std::cerr << msg << std::endl;
        return 1;
    }

    /*************************************************
     * At this point we are logged on to the inverter
     *************************************************/

    return rc;
}

void Inverter::logOff()
{
// lol
}

/*
E_SBFSPOT Inverter::getPacket(unsigned char senderaddr[6], int wait4Command)
{
    if (DEBUG_NORMAL) printf("getPacket(%d)\n", wait4Command);
    int index = 0;
    int hasL2pckt  = 0;
    E_SBFSPOT rc = E_OK;
    pkHeader *pkHdr = (pkHeader *)CommBuf;
    do
    {
        int bib = bthRead(CommBuf, sizeof(pkHeader));
        if (bib <= 0)
        {
            printf("No data!\n");
            return E_NODATA;
        }

        //More data after header?
        if (btohs(pkHdr->pkLength) > sizeof(pkHeader))
        {
            bib += bthRead(CommBuf + sizeof(pkHeader), btohs(pkHdr->pkLength) - sizeof(pkHeader));

            HexDump(CommBuf, bib, 10);

            //Check if data is coming from the right inverter
            if (isValidSender(senderaddr, pkHdr->SourceAddr) == 1)
            {
                rc = E_OK;
                printf("cmd=%d\n", btohs(pkHdr->command));

                if ((hasL2pckt == 0) && (CommBuf[18] == 0x7E) && (get_long(CommBuf+19) == 0x656003FF))
                {
                    hasL2pckt = 1;
                }

                if (hasL2pckt == 1) {
                    //Copy CommBuf to packetbuffer
                    int bufptr = sizeof(pkHeader);
                    bool escNext = false;

                    printf("PacketLength=%d\n", btohs(pkHdr->pkLength));

                    for (int i = sizeof(pkHeader); i < btohs(pkHdr->pkLength); i++) {
                        pcktBuf[index] = CommBuf[bufptr++];
                        //Keep 1st byte raw unescaped 0x7E
                        if (escNext == true) {
                            pcktBuf[index] ^= 0x20;
                            escNext = false;
                            index++;
                        } else {
                            if (pcktBuf[index] == 0x7D)
                                escNext = true; //Throw away the 0x7d byte
                            else
                                index++;
                        }
                        if (index >= maxpcktBufsize) {
                            printf("Warning: pcktBuf buffer overflow! (%d)\n", index);
                            return E_BUFOVRFLW;
                        }
                    }
                    packetposition = index;
                }
                else
                {
                    memcpy(pcktBuf, CommBuf, bib);
                    packetposition = bib;
                }
            } // isValidSender()
            else
            {
                rc = E_RETRY;
                if (DEBUG_NORMAL)
                    printf("Wrong sender: %02X:%02X:%02X:%02X:%02X:%02X\n",
                           pkHdr->SourceAddr[5],
                           pkHdr->SourceAddr[4],
                           pkHdr->SourceAddr[3],
                           pkHdr->SourceAddr[2],
                           pkHdr->SourceAddr[1],
                           pkHdr->SourceAddr[0]);
            }
        }
        else
        {
            HexDump(CommBuf, bib, 10);
            //Check if data is coming from the right inverter
            if (isValidSender(senderaddr, pkHdr->SourceAddr) == 1)
            {
                rc = E_OK;
                if (DEBUG_NORMAL) printf("cmd=%d\n", btohs(pkHdr->command));

                memcpy(pcktBuf, CommBuf, bib);
                packetposition = bib;
            } // isValidSender()
            else
            {
                rc = E_RETRY;
                if (DEBUG_NORMAL)
                    printf("Wrong sender: %02X:%02X:%02X:%02X:%02X:%02X\n",
                           pkHdr->SourceAddr[5],
                           pkHdr->SourceAddr[4],
                           pkHdr->SourceAddr[3],
                           pkHdr->SourceAddr[2],
                           pkHdr->SourceAddr[1],
                           pkHdr->SourceAddr[0]);
            }
        }
    }
    // changed to have "any" wait4Command (0xFF) - if you have different order of commands
    while (((btohs(pkHdr->command) != wait4Command) || (rc == E_RETRY)) && (0xFF != wait4Command));

    if (rc == E_OK) {
        printf("<<<====== Content of pcktBuf =======>>>\n");
        HexDump(pcktBuf, packetposition, 10);
        printf("<<<=================================>>>\n");
    }

    if (packetposition > MAX_pcktBuf) {
        MAX_pcktBuf = packetposition;
        printf("MAX_pcktBuf is now %d bytes\n", MAX_pcktBuf);
    }

    return rc;
}*/

E_SBFSPOT Inverter::logonSMAInverter(long userGroup, const char *password)
{
#define MAX_PWLENGTH 12
    unsigned char pw[MAX_PWLENGTH] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    if (DEBUG_NORMAL) puts("logonSMAInverter()");

    char encChar = (userGroup == UG_USER)? 0x88:0xBB;
    //Encode password
    unsigned int idx;
    for (idx = 0; (password[idx] != 0) && (idx < sizeof(pw)); idx++)
        pw[idx] = password[idx] + encChar;
    for (; idx < MAX_PWLENGTH; idx++)
        pw[idx] = encChar;

    E_SBFSPOT rc = E_OK;
    int validPcktID = 0;

    time_t now;

    // WARNING This might be an issue when dealing with multiple inverters?
    pcktID++;
    now = time(NULL);
    writePacketHeader(pcktBuf, 0x01, addr_unknown);
    if (SUSyID != SID_SB240)
        writePacket(pcktBuf, 0x0E, 0xA0, 0x0100, SUSyID, Serial);
    else
        writePacket(pcktBuf, 0x0E, 0xE0, 0x0100, SUSyID, Serial);

    writeLong(pcktBuf, 0xFFFD040C);
    writeLong(pcktBuf, userGroup);    // User / Installer
    writeLong(pcktBuf, 0x00000384); // Timeout = 900sec ?
    writeLong(pcktBuf, now);
    writeLong(pcktBuf, 0);
    writeArray(pcktBuf, pw, sizeof(pw));
    writePacketTrailer(pcktBuf);
    writePacketLength(pcktBuf);

    ethSend(pcktBuf, IPAddress.c_str());

    validPcktID = 0;
    do
    {
        if ((rc = ethGetPacket()) == E_OK)
        {
            ethPacket *pckt = (ethPacket *)pcktBuf;
            if (pcktID == (btohs(pckt->PacketID) & 0x7FFF))   // Valid Packet ID
            {
                validPcktID = 1;
                unsigned short retcode = btohs(pckt->ErrorCode);
                switch (retcode)
                {
                case 0:
                    rc = E_OK;
                    break;
                case 0x0100:
                    rc = E_INVPASSW;
                    break;
                default:
                    rc = E_LOGONFAILED;
                    break;
                }
            }
            else
                printf("Packet ID mismatch. Expected %d, received %d\n", pcktID, (btohs(pckt->PacketID) & 0x7FFF));
        }
    } while ((validPcktID == 0) && (rc == E_OK)); // Fix Issue 167

    return rc;
}

E_SBFSPOT Inverter::logoffSMAInverter()
{
    if (DEBUG_NORMAL) puts("logoffSMAInverter()");

    pcktID++;
    writePacketHeader(pcktBuf, 0x01, addr_unknown);
    writePacket(pcktBuf, 0x08, 0xA0, 0x0300, anySUSyID, anySerial);
    writeLong(pcktBuf, 0xFFFD010E);
    writeLong(pcktBuf, 0xFFFFFFFF);
    writePacketTrailer(pcktBuf);
    writePacketLength(pcktBuf);

    ethSend(pcktBuf, IPAddress.c_str());

    return E_OK;
}

void debug_watt(const char *txt, const int32_t val, const time_t dt)
{
    if (DEBUG_NORMAL)
    {
        printf("%-12s: %d (W) %s", txt, val, ctime(&dt));
    }
}

void debug_volt(const char *txt, const int32_t val, const time_t dt)
{
    if (DEBUG_NORMAL)
    {
        printf("%-12s: %.2f (V) %s", txt, toVolt(val), ctime(&dt));
    }
}

void debug_amp(const char *txt, const int32_t val, const time_t dt)
{
    if (DEBUG_NORMAL)
    {
        printf("%-12s: %.3f (A) %s", txt, toAmp(val), ctime(&dt));
    }
}

void debug_hz(const char *txt, const int32_t val, const time_t dt)
{
    if (DEBUG_NORMAL)
    {
        printf("%-12s: %.2f (Hz) %s", txt, toHz(val), ctime(&dt));
    }
}

void debug_kwh(const char *txt, const uint64_t val, const time_t dt)
{
    if (DEBUG_NORMAL)
    {
        printf("%-12s: %.3f (kWh) %s", txt, tokWh(val), ctime(&dt));
    }
}

void debug_hour(const char *txt, const uint64_t val, const time_t dt)
{
    if (DEBUG_NORMAL)
    {
        printf("%-12s: %.3f (h) %s", txt, toHour(val), ctime(&dt));
    }
}

void debug_text(const char *txt, const char *val, const time_t dt)
{
    if (DEBUG_NORMAL)
    {
        printf("%-12s: '%s' %s", txt, val, ctime(&dt));
    }
}

int Inverter::getInverterData(enum getInverterDataType type)
{
    int rc = E_OK;
    unsigned long command;
    unsigned long first;
    unsigned long last;

    /**
     * Protocol setup
     */
    switch (type) {
    case Type_EnergyProduction:
        // SPOT_ETODAY, SPOT_ETOTAL
        command = 0x54000200;
        first = 0x00260100;
        last = 0x002622FF;
        break;

    case Type_SpotDCPower:
        // SPOT_PDC1, SPOT_PDC2
        command = 0x53800200;
        first = 0x00251E00;
        last = 0x00251EFF;
        break;

    case Type_SpotDCVoltage:
        // SPOT_UDC1, SPOT_UDC2, SPOT_IDC1, SPOT_IDC2
        command = 0x53800200;
        first = 0x00451F00;
        last = 0x004521FF;
        break;

    case Type_SpotACPower:
        // SPOT_PAC1, SPOT_PAC2, SPOT_PAC3
        command = 0x51000200;
        first = 0x00464000;
        last = 0x004642FF;
        break;

    case Type_SpotACVoltage:
        // SPOT_UAC1, SPOT_UAC2, SPOT_UAC3, SPOT_IAC1, SPOT_IAC2, SPOT_IAC3
        command = 0x51000200;
        first = 0x00464800;
        last = 0x004655FF;
        break;

    case Type_SpotGridFrequency:
        // SPOT_FREQ
        command = 0x51000200;
        first = 0x00465700;
        last = 0x004657FF;
        break;

    case Type_SpotACTotalPower:
        // SPOT_PACTOT
        command = 0x51000200;
        first = 0x00263F00;
        last = 0x00263FFF;
        break;

    case Type_TypeLabel:
        // INV_NAME, INV_TYPE, INV_CLASS
        command = 0x58000200;
        first = 0x00821E00;
        last = 0x008220FF;
        break;

    case Type_SoftwareVersion:
        // INV_SWVERSION
        command = 0x58000200;
        first = 0x00823400;
        last = 0x008234FF;
        break;

    case Type_DeviceStatus:
        // INV_STATUS
        command = 0x51800200;
        first = 0x00214800;
        last = 0x002148FF;
        break;

    case Type_GridRelayStatus:
        // INV_GRIDRELAY
        command = 0x51800200;
        first = 0x00416400;
        last = 0x004164FF;
        break;

    case Type_OperationTime:
        // SPOT_OPERTM, SPOT_FEEDTM
        command = 0x54000200;
        first = 0x00462E00;
        last = 0x00462FFF;
        break;

    case Type_BatteryChargeStatus:
        command = 0x51000200;
        first = 0x00295A00;
        last = 0x00295AFF;
        break;

    case Type_BatteryInfo:
        command = 0x51000200;
        first = 0x00491E00;
        last = 0x00495DFF;
        break;

    case Type_InverterTemperature:
        command = 0x52000200;
        first = 0x00237700;
        last = 0x002377FF;
        break;

    case Type_sbftest:
        command = 0x64020200;
        first = 0x00618D00;
        last = 0x00618DFF;
        break;

    case Type_MeteringGridMsTotW:
        command = 0x51000200;
        first = 0x00463600;
        last = 0x004637FF;
        break;

    default:
        return E_BADARG;
    };

    pcktID++;
    writePacketHeader(pcktBuf, 0x01, addr_unknown);
    if (SUSyID == SID_SB240)
        writePacket(pcktBuf, 0x09, 0xE0, 0, SUSyID, Serial);
    else
        writePacket(pcktBuf, 0x09, 0xA0, 0, SUSyID, Serial);
    writeLong(pcktBuf, command);
    writeLong(pcktBuf, first);
    writeLong(pcktBuf, last);
    writePacketTrailer(pcktBuf);
    writePacketLength(pcktBuf);

    ethSend(pcktBuf, IPAddress.c_str());

    do {
        do {
            rc = ethGetPacket();

            if (rc != E_OK)
                return rc;

            uint16_t status = get_short(pcktBuf + 23);
            if (status != 0) {
                printf("Packet status: %d\n", status);
                return status;
            }

            pcktcount = get_short(pcktBuf + 25);
            unsigned short rcvpcktID = get_short(pcktBuf + 27) & 0x7FFF;
            if (pcktID == rcvpcktID)
            {
                int inv = getInverterIndexBySerial(get_short(pcktBuf + 15), get_long(pcktBuf + 17));
                if (inv >= 0)
                {
                    validPcktID = 1;
                    int32_t value = 0;
                    int64_t value64 = 0;
                    uint32_t recordsize = 4 * ((uint32_t)pcktBuf[5] - 9) / ((uint32_t)get_long(pcktBuf + 37) - (uint32_t)get_long(pcktBuf + 33) + 1);

                    for (int ii = 41; ii < packetposition - 3; ii += recordsize)
                    {
                        unsigned char *recptr = pcktBuf + ii;
                        uint32_t code = ((uint32_t)get_long(recptr));
                        LriDef lri = (LriDef)(code & 0x00FFFF00);
                        uint32_t cls = code & 0xFF;
                        unsigned char dataType = code >> 24;
                        time_t datetime = (time_t)get_long(recptr + 4);

                        // fix: We can't rely on dataType because it can be both 0x00 or 0x40 for DWORDs
                        //if ((lri == MeteringDyWhOut) || (lri == MeteringTotWhOut) || (lri == MeteringTotFeedTms) || (lri == MeteringTotOpTms))  //QWORD
                        if (recordsize == 16)
                        {
                            value64 = get_longlong(recptr + 8);
                            if (is_NaN(value64) || is_NaN((uint64_t)value64))
                                value64 = 0;
                        }
                        else if ((dataType != DT_STRING) && (dataType != DT_STATUS))
                        {
                            value = get_long(recptr + 16);
                            if (is_NaN(value) || is_NaN((uint32_t)value))
                                value = 0;
                        }

                        switch (lri)
                        {
                        case GridMsTotW: //SPOT_PACTOT
                            //This function gives us the time when the inverter was switched off
                            SleepTime = datetime;
                            TotalPac = value;
                            flags |= type;
                            debug_watt("SPOT_PACTOT", value, datetime);
                            break;

                        case GridMsWphsA: //SPOT_PAC1
                            Pac1 = value;
                            flags |= type;
                            debug_watt("SPOT_PAC1", value, datetime);
                            break;

                        case GridMsWphsB: //SPOT_PAC2
                            Pac2 = value;
                            flags |= type;
                            debug_watt("SPOT_PAC2", value, datetime);
                            break;

                        case GridMsWphsC: //SPOT_PAC3
                            Pac3 = value;
                            flags |= type;
                            debug_watt("SPOT_PAC3", value, datetime);
                            break;

                        case GridMsPhVphsA: //SPOT_UAC1
                            Uac1 = value;
                            flags |= type;
                            debug_volt("SPOT_UAC1", value, datetime);
                            break;

                        case GridMsPhVphsB: //SPOT_UAC2
                            Uac2 = value;
                            flags |= type;
                            debug_volt("SPOT_UAC2", value, datetime);
                            break;

                        case GridMsPhVphsC: //SPOT_UAC3
                            Uac3 = value;
                            flags |= type;
                            debug_volt("SPOT_UAC3", value, datetime);
                            break;

                        case GridMsAphsA_1: //SPOT_IAC1
                        case GridMsAphsA:
                            Iac1 = value;
                            flags |= type;
                            debug_amp("SPOT_IAC1", value, datetime);
                            break;

                        case GridMsAphsB_1: //SPOT_IAC2
                        case GridMsAphsB:
                            Iac2 = value;
                            flags |= type;
                            debug_amp("SPOT_IAC2", value, datetime);
                            break;

                        case GridMsAphsC_1: //SPOT_IAC3
                        case GridMsAphsC:
                            Iac3 = value;
                            flags |= type;
                            debug_amp("SPOT_IAC3", value, datetime);
                            break;

                        case GridMsHz: //SPOT_FREQ
                            GridFreq = value;
                            flags |= type;
                            debug_amp("SPOT_FREQ", value, datetime);
                            break;

                        case DcMsWatt: //SPOT_PDC1 / SPOT_PDC2
                            // TODO: Remove cls 1/2
                            if (cls == 1)
                            {
                                Pdc1 = value;
                            }
                            if (cls == 2)
                            {
                                Pdc2 = value;
                            }

                            /*
                            it = mpp.find((uint8_t)cls);
                            if (it != mpp.end())
                                it->second.Pdc(value);
                            else
                            {
                                mppt new_mppt;
                                new_mppt.Pdc(value);
                                mpp.insert(std::make_pair(cls, new_mppt));
                            }*/

                            debug_watt((std::string("SPOT_PDC") + std::to_string(cls)).c_str(), value, datetime);

                            calPdcTot += value;

                            flags |= type;
                            break;

                        case DcMsVol: //SPOT_UDC1 / SPOT_UDC2
                            // TODO: Remove cls 1/2
                            if (cls == 1)
                            {
                                Udc1 = value;
                            }
                            if (cls == 2)
                            {
                                Udc2 = value;
                            }

                            it = mpp.find((uint8_t)cls);
                            if (it != mpp.end())
                                it->second.Udc(value);
                            else
                            {
                                mppt new_mppt;
                                new_mppt.Udc(value);
                                mpp.insert(std::make_pair(cls, new_mppt));
                            }

                            debug_volt((std::string("SPOT_UDC") + std::to_string(cls)).c_str(), value, datetime);

                            flags |= type;
                            break;

                        case DcMsAmp: //SPOT_IDC1 / SPOT_IDC2
                            // TODO: Remove cls 1/2
                            if (cls == 1)
                            {
                                Idc1 = value;
                            }
                            if (cls == 2)
                            {
                                Idc2 = value;
                            }

                            it = mpp.find((uint8_t)cls);
                            if (it != mpp.end())
                                it->second.Idc(value);
                            else
                            {
                                mppt new_mppt;
                                new_mppt.Idc(value);
                                mpp.insert(std::make_pair(cls, new_mppt));
                            }

                            debug_amp((std::string("SPOT_IDC") + std::to_string(cls)).c_str(), value, datetime);

                            flags |= type;
                            break;

                        case MeteringTotWhOut: //SPOT_ETOTAL
                            //In case SPOT_ETODAY missing, this function gives us inverter time (eg: SUNNY TRIPOWER 6.0)
                            InverterDatetime = datetime;
                            ETotal = value64;
                            flags |= type;
                            debug_kwh("SPOT_ETOTAL", value64, datetime);
                            break;

                        case MeteringDyWhOut: //SPOT_ETODAY
                            //This function gives us the current inverter time
                            InverterDatetime = datetime;
                            EToday = value64;
                            flags |= type;
                            debug_kwh("SPOT_ETODAY", value64, datetime);
                            break;

                        case MeteringTotOpTms: //SPOT_OPERTM
                            OperationTime = value64;
                            flags |= type;
                            debug_hour("SPOT_OPERTM", value64, datetime);
                            break;

                        case MeteringTotFeedTms: //SPOT_FEEDTM
                            FeedInTime = value64;
                            flags |= type;
                            debug_hour("SPOT_FEEDTM", value64, datetime);
                            break;

                        case NameplateLocation: //INV_NAME
                            //This function gives us the time when the inverter was switched on
                            WakeupTime = datetime;
                            DeviceName = std::string((char *)recptr + 8, strnlen((char *)recptr + 8, recordsize - 8)); // Fix #506
                            flags |= type;
                            debug_text("INV_NAME", DeviceName.c_str(), datetime);
                            break;

                        case NameplatePkgRev: //INV_SWVER
                            SWVersion = version_tostring(get_long(recptr + 24));
                            flags |= type;
                            debug_text("INV_SWVER", SWVersion.c_str(), datetime);
                            break;

                        case NameplateModel: //INV_TYPE
                        {
                            auto attr = getattribute(recptr);
                            if (attr.size() > 0)
                            {
                                DeviceType = tagdefs.getDesc(attr.front());
                                if (DeviceType.empty())
                                {
                                    DeviceType = "UNKNOWN TYPE";
                                    printf("ID='%d'\n", attr.front());
                                }
                                flags |= type;
                                debug_text("INV_TYPE", DeviceType.c_str(), datetime);
                            }
                            break;
                        }

                        case NameplateMainModel: //INV_CLASS
                        {
                            auto attr = getattribute(recptr);
                            if (attr.size() > 0)
                            {
                                DevClass = (DEVICECLASS)attr.front();
                                DeviceClass = tagdefs.getDesc(DevClass, "UNKNOWN CLASS");

                                flags |= type;
                                debug_text("INV_CLASS", DeviceClass.c_str(), datetime);
                            }
                            break;
                        }

                        case OperationHealth: //INV_STATUS:
                        {
                            auto attr = getattribute(recptr);
                            if (attr.size() > 0)
                            {
                                DeviceStatus = attr.front();
                                flags |= type;
                                debug_text("INV_STATUS", tagdefs.getDesc(DeviceStatus, "?").c_str(), datetime);
                            }
                            break;
                        }

                        case OperationGriSwStt: //INV_GRIDRELAY
                        {
                            auto attr = getattribute(recptr);
                            if (attr.size() > 0)
                            {
                                GridRelayStatus = attr.front();
                                flags |= type;
                                debug_text("INV_GRIDRELAY", tagdefs.getDesc(GridRelayStatus, "?").c_str(), datetime);
                            }
                            break;
                        }

                        case BatChaStt:
                            BatChaStt = value;
                            flags |= type;
                            break;

                        case BatDiagCapacThrpCnt:
                            BatDiagCapacThrpCnt = value;
                            flags |= type;
                            break;

                        case BatDiagTotAhIn:
                            BatDiagTotAhIn = value;
                            flags |= type;
                            break;

                        case BatDiagTotAhOut:
                            BatDiagTotAhOut = value;
                            flags |= type;
                            break;

                        case BatTmpVal:
                            BatTmpVal = value;
                            flags |= type;
                            break;

                        case BatVol:
                            BatVol = value;
                            flags |= type;
                            break;

                        case BatAmp:
                            BatAmp = value;
                            flags |= type;
                            break;

                        case CoolsysTmpNom:
                            Temperature = value;
                            flags |= type;
                            break;

                        case MeteringGridMsTotWOut:
                            MeteringGridMsTotWOut = value;
                            break;

                        case MeteringGridMsTotWIn:
                            MeteringGridMsTotWIn = value;
                            break;

                        default:
                            /*
                             // tagdefs -> messages
                            switch (dataType)
                            {
                            case DT_ULONG:
                                if (recordsize == 16)
                                {
                                    printf("%08X %d %s '%s' %s\n", code, recordsize, strtok(ctime(&datetime), "\n"), tagdefs.getDescForLRI(lri).c_str(), u64_tostring(get_longlong(recptr + 8)).c_str());
                                }
                                else if (recordsize == 28)
                                {
                                    printf("%08X %d %s '%s' %s %s %s %s\n", code, recordsize, strtok(ctime(&datetime), "\n"), tagdefs.getDescForLRI(lri).c_str(),
                                           u32_tostring(get_long(recptr + 8)).c_str(),
                                           u32_tostring(get_long(recptr + 12)).c_str(),
                                           u32_tostring(get_long(recptr + 16)).c_str(),
                                           u32_tostring(get_long(recptr + 20)).c_str()
                                          );
                                }
                                else if (recordsize == 40)
                                {
                                    printf("%08X %d %s '%s' %s %s %s %s %s %s\n", code, recordsize, strtok(ctime(&datetime), "\n"), tagdefs.getDescForLRI(lri).c_str(),
                                           u32_tostring(get_long(recptr + 8)).c_str(),
                                           u32_tostring(get_long(recptr + 12)).c_str(),
                                           u32_tostring(get_long(recptr + 16)).c_str(),
                                           u32_tostring(get_long(recptr + 20)).c_str(),
                                           u32_tostring(get_long(recptr + 24)).c_str(),
                                           u32_tostring(get_long(recptr + 28)).c_str()
                                          );
                                }
                                else
                                    printf("%08X ?%d? %s '%s'\n", code, recordsize, strtok(ctime(&datetime), "\n"), tagdefs.getDescForLRI(lri).c_str());
                                break;

                            case DT_STATUS:
                            {
                                for (const auto &tag : getattribute(recptr))
                                    printf("%08X %d %s %s: '%s'\n", code, recordsize, strtok(ctime(&datetime), "\n"), tagdefs.getDescForLRI(lri).c_str(), tagdefs.getDesc(tag,"???").c_str());
                            }
                            break;

                            case DT_STRING:
                            {
                                char str[40];
                                strncpy(str, (char*)recptr + 8, recordsize - 8);
                                printf("%08X %d %s %s: '%s'\n", code, recordsize, strtok(ctime(&datetime), "\n"), tagdefs.getDescForLRI(lri).c_str(), str);
                            }
                            break;

                            case DT_SLONG:
                                if (recordsize == 16)
                                {
                                    printf("%08X %d %s '%s' %s\n", code, recordsize, strtok(ctime(&datetime), "\n"), tagdefs.getDescForLRI(lri).c_str(), s64_tostring(get_longlong(recptr + 8)).c_str());
                                }
                                else if (recordsize == 28)
                                {
                                    printf("%08X %d %s '%s' %s %s %s %s\n", code, recordsize, strtok(ctime(&datetime), "\n"), tagdefs.getDescForLRI(lri).c_str(),
                                           s32_tostring(get_long(recptr + 8)).c_str(),
                                           s32_tostring(get_long(recptr + 12)).c_str(),
                                           s32_tostring(get_long(recptr + 16)).c_str(),
                                           s32_tostring(get_long(recptr + 20)).c_str()
                                          );

                                }
                                else if (recordsize == 40)
                                {
                                    printf("%08X %d %s '%s' %s %s %s %s %s %s\n", code, recordsize, strtok(ctime(&datetime), "\n"), tagdefs.getDescForLRI(lri).c_str(),
                                           s32_tostring(get_long(recptr + 8)).c_str(),
                                           s32_tostring(get_long(recptr + 12)).c_str(),
                                           s32_tostring(get_long(recptr + 16)).c_str(),
                                           s32_tostring(get_long(recptr + 20)).c_str(),
                                           s32_tostring(get_long(recptr + 24)).c_str(),
                                           s32_tostring(get_long(recptr + 28)).c_str()
                                          );
                                }
                                else
                                    printf("%08X ?%d? %s '%s'\n", code, recordsize, strtok(ctime(&datetime), "\n"), tagdefs.getDescForLRI(lri).c_str());
                                break;

                            default:
                                printf("%08X %d %s '%s'\n", code, recordsize, strtok(ctime(&datetime), "\n"), tagdefs.getDescForLRI(lri).c_str());
                                break;
                            }*/
                            break;
                        }
                    }
                }
            }
            else
            {
                if (DEBUG_HIGHEST) printf("Packet ID mismatch. Expected %d, received %d\n", pcktID, rcvpcktID);
                validPcktID = 0;
                pcktcount = 0;
            }

        } while (pcktcount > 0);
    } while (validPcktID ==  0);

    return rc;
}

char *strftime_t (const time_t rawtime)
{
    static char buffer[256];
    struct tm tm_struct;
    memcpy(&tm_struct, localtime(&rawtime), sizeof(tm_struct));
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %X", &tm_struct);
    return buffer;
}

