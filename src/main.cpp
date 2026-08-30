/** Includes */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h> // for sleep()
#include <vector>

#include <error_codes.h>

#include <1_LL/network/network.h>
#include <2_DRIVERS/database/influx.hpp>
#include <2_DRIVERS/database/influxline.hpp>
#include <2_DRIVERS/modbus/modbus.h>
#include <3_APPLICATION/config/config.hpp>
#include <3_APPLICATION/inverter/inverter.hpp>
#include <3_APPLICATION/modbus/modbus_sma.h>
#include <3_APPLICATION/modbus/sma.h>

/** Globals */
bool g_debug = false;

/** Prototypes */
static error_e app_run(const Configuration &cfg, influx::Influx &ifx, std::vector<SmaInverter_t> &aInverters);
static error_e app_cleanup(void);
static FIX0 getFix0AtOffset(uint32_t* pArray, uint16_t baseAddress, uint16_t targetAddress);
static error_e getValuesFromInverter(SmaInverter_t&);
static error_e exportToInflux(influx::Influx&, std::vector<SmaInverter_t> &, unsigned long);
static InfluxLine updateInfluxLine(const SmaInverter_t &inv, unsigned long currentTimestamp);

/** Public functions */
/**
 * @brief main function
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char* argv[])
{
    using namespace std;

    /* The current InfluxDB config */
    Configuration cfg {};

    if (eError_ok != getConfiguration(cfg)) {
        exit(EXIT_FAILURE);
    }

    /** Set globals */
    g_debug = cfg.debug;

    /* Init HTTP */
    if (eError_ok != influx::init()) {
        exit(EXIT_FAILURE);
    }

    {
        /** Connect to InfluxDB */
        influx::Influx ifx(cfg.influx.host, cfg.influx.port, cfg.influx.org, cfg.influx.bucket, cfg.influx.token, cfg.influx.measurement);

        cout << "Connecting to Inverters...\n";
        std::vector<SmaInverter_t> aInverters;

        /* For each configured inverter */
        for (const InverterConfig& invCfg : cfg.inverters) {
            aInverters.push_back((SmaInverter_t) {
                .networkHandle = network_init(cfg.timeout),
                .unitIdentifier = static_cast<uint8_t>(invCfg.unitIdentifier),
                .name = invCfg.name,
            });

            /* Connect to the inverter */
            auto inv = aInverters.back();
            if (eError_ok != network_connect(inv.networkHandle, invCfg.ip.c_str(), eModbus_port)) {
                cerr << "Couldn't connect to inverter\n";
                return eError_failed;
            }
        };

        app_run(cfg, ifx, aInverters);
        for (SmaInverter_t& inv : aInverters) {
            network_close(inv.networkHandle);
            network_deinit(inv.networkHandle);
        }
    }

    app_cleanup();
    
    return EXIT_FAILURE;
}

static error_e app_run(const Configuration &cfg, influx::Influx &ifx, std::vector<SmaInverter_t> &aInverters)
{
    using namespace std;

    /* Main loop */
    for (;;) {

        /* Divide by interval to have a rounded timestamp */
        unsigned long currentTimestamp = time(NULL);
        currentTimestamp = currentTimestamp - (currentTimestamp % cfg.interval);

        for (SmaInverter_t& inv : aInverters) {

            if (getValuesFromInverter(inv) != eError_ok) {
                cerr << "Processing inverter failed " << endl;
                return eError_failed;
            }
        }

        if (cfg.debug) {

            for (SmaInverter_t& inv : aInverters) {
                cout << to_string(inv) << "\n";
            }
        }

        /** Export to InfluxDB using the same timestamp */
        if (eError_ok != exportToInflux(ifx, aInverters, currentTimestamp)) {
            cerr << "Influx failed" << endl;
            return eError_failed;
        }

        sleep(cfg.interval);
    }
}

/**
 * @brief cleans up any globals
 * @return error_e status
 */
static error_e app_cleanup(void)
{
    return influx::deinit();
}

/**
 * @brief assumes pArray is big enough, retrieves the value from the \p pArray with a base address of \p baseAddress
 * @param pArray
 * @param baseAddress
 * @param targetAddress
 * @return FIX0
 */
static FIX0 getFix0AtOffset(uint32_t* pArray, uint16_t baseAddress, uint16_t targetAddress)
{
    return *(pArray + ((targetAddress - baseAddress) / 2));
}

/**
 * @brief requests everything needed from an inverter and pushes to influxDB
 * @param[in,out] inv		inverter struct with IP already filled in
 * @retval eError_ok		if successfully processed inverter
 * @retval eError_failed	if unsuccessfull to process the given inverter
 */
static error_e getValuesFromInverter(SmaInverter_t& inv)
{
    /* Prepare handle */
    modbusWrapper_handle_t handle = {
        .unitIdentifier = inv.unitIdentifier,
        .pNetworkHandle = inv.networkHandle,
    };

    uint32_t value = 0;

    /* Get inverter state */
    if (eError_ok != fetchU32(&handle, eSmaModbusRegister_pvSystemUtilityGridConnection, &value)) {
        return eError_failed;
    }
    inv.gridConnection = static_cast<smaModbus_pvSystemUtilityGridConnection_e>(value);

    /* Status of the device */
    if (eError_ok != fetchU32(&handle, eSmaModbusRegister_statusOfDevice, &value)) {
        return eError_failed;
    }
    inv.statusOfDevice = static_cast<smaModbus_statusOfTheDevice_e>(value);

    /* Energy */
    if (eError_ok != fetchU32(&handle, eSmaModbusRegister_totalAcEnergyFedInOnAllLineConductors_Wh, &value)) {
        return eError_failed;
    }
    inv.totalYield = value;

    /* Day Energy */
    if (eError_ok != fetchU32(&handle, eSmaModbusRegister_energyFedInOnTheCurrentDayOnAllLineConductors_Wh, &value)) {
        return eError_failed;
    }
    inv.dayYield = value;

    /* Get U, I, P */
    uint32_t aRegisters[32];
    if (eError_ok != fetchU32Multiple(&handle, eSmaModbusRegister_dcCurrentInput1_S32, (eSmaModbusRegister_lineCurrentOnAllLineConductors - eSmaModbusRegister_dcCurrentInput1_S32) / 2 + 1, aRegisters)) {
        return eError_failed;
    }

    /** DC current input 1 (A) S32; FIX3 */
    inv.idc1 = ((double)getFix0AtOffset(aRegisters, eSmaModbusRegister_dcCurrentInput1_S32, eSmaModbusRegister_dcCurrentInput1_S32) / 1000);
    /** DC voltage input 1 (V) S32; FIX2 */
    inv.udc1 = ((double)getFix0AtOffset(aRegisters, eSmaModbusRegister_dcCurrentInput1_S32, eSmaModbusRegister_dcVoltageInput1_S32) / 100);
    /** DC power input 1 (W) S32; FIX0 */
    inv.pdc1 = getFix0AtOffset(aRegisters, eSmaModbusRegister_dcCurrentInput1_S32, eSmaModbusRegister_dcPowerInput1_S32);
    /** Active power of line conductor L1 (W) S32; FIX0 */
    inv.pac1 = getFix0AtOffset(aRegisters, eSmaModbusRegister_dcCurrentInput1_S32, eSmaModbusRegister_activePowerOnallLineConductors);
    /** Line voltage, line conductor L1 to N (V) U32; FIX2 */
    inv.uac1 = ((double)getFix0AtOffset(aRegisters, eSmaModbusRegister_dcCurrentInput1_S32, eSmaModbusRegister_lineVoltageLineConductorL1ToN) / 100);
    /** Line current of line conductor L1 (A); S32; FIX3 */
    inv.iac1 = ((double)getFix0AtOffset(aRegisters, eSmaModbusRegister_dcCurrentInput1_S32, eSmaModbusRegister_lineCurrentOnAllLineConductors) / 1000);

    /* Grid Frequency, reactive & apparent power */
    if (eError_ok != fetchU32Multiple(&handle, eSmaModbusRegister_powerFrequency, (eSmaModbusRegister_apparentPowerOnAllLineConductors - eSmaModbusRegister_powerFrequency) / 2 + 1, aRegisters)) {
        return eError_failed;
    }

    inv.gridFreq = (double)getFix0AtOffset(aRegisters, eSmaModbusRegister_powerFrequency, eSmaModbusRegister_powerFrequency) / 100;
    inv.reactivePower = getFix0AtOffset(aRegisters, eSmaModbusRegister_powerFrequency, eSmaModbusRegister_reactivePowerOnAllLineConductors);
    inv.apparentPower = getFix0AtOffset(aRegisters, eSmaModbusRegister_powerFrequency, eSmaModbusRegister_apparentPowerOnAllLineConductors);

    /** Temperature, DC */
    if (eError_ok != fetchU32Multiple(&handle, eSmaModbusRegister_internalTemperature, (eSmaModbusRegister_dcPowerInput2_S32 - eSmaModbusRegister_internalTemperature) / 2 + 1, aRegisters)) {
        return eError_failed;
    }

    inv.temperature = getFix0AtOffset(aRegisters, eSmaModbusRegister_internalTemperature, eSmaModbusRegister_internalTemperature) / 10;
    inv.idc2 = (double)getFix0AtOffset(aRegisters, eSmaModbusRegister_internalTemperature, eSmaModbusRegister_dcCurrentInput2_S32) / 1000;
    inv.udc2 = (double)getFix0AtOffset(aRegisters, eSmaModbusRegister_internalTemperature, eSmaModbusRegister_dcVoltageInput2_S32) / 100;
    inv.pdc2 = getFix0AtOffset(aRegisters, eSmaModbusRegister_internalTemperature, eSmaModbusRegister_dcPowerInput2_S32);

    return eError_ok;
}

/**
 * @brief constructs the line protocol to send to Influx
 * @param ifx
 * @param inv
 * @param currentTimestamp
 * @return error_e from Influx.post()
 */
static error_e exportToInflux(influx::Influx& ifx, std::vector<SmaInverter_t> &aInverters,
    unsigned long currentTimestamp)
{
    std::string influxLines;

    /* Create Line for Influx for each Inverter */
    for (SmaInverter_t &inv : aInverters) {
        std::string line = updateInfluxLine(inv, currentTimestamp).getLine();

        /* New line for each measurement */
        influxLines += line + "\n";
    }

    return ifx.post(influxLines);
}


static InfluxLine updateInfluxLine(const SmaInverter_t &inv, unsigned long currentTimestamp) {
    /* Create object */
    InfluxLine line("inverter");

    line.setTimestamp(currentTimestamp);

    line.addTag("name", inv.name);

    line.addField("status", int(inv.statusOfDevice));
    line.addField("gridConnection", int(inv.gridConnection));

    line.addField("dayYield", int(inv.dayYield));
    line.addField("totalYield", int(inv.totalYield));

    /* Inverter's grid relay contactor is not closed -> Post only limited values, should find some other way*/
    if (eSmaModbusPvSystemUtilityGridConnection_utilityGrid == inv.gridConnection) {
        line.addField("temperature", inv.temperature);

        line.addField("gridFreq", inv.gridFreq);

        line.addField("pac1", int(inv.pac1));
        line.addField("pdc1", int(inv.pdc1));
        line.addField("pdc2", int(inv.pdc2));

        line.addField("uac1", inv.uac1);
        line.addField("udc1", inv.udc1);
        line.addField("udc2", inv.udc2);

        line.addField("iac1", inv.iac1);
        line.addField("idc1", inv.idc1);
        line.addField("idc2", inv.idc2);

        line.addField("reactivePower", int(inv.reactivePower));
        line.addField("apparentPower", int(inv.apparentPower));

    }
    
    return line;
}