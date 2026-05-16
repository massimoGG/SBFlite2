/** Includes */
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h> // for sleep()

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
error_e getValuesFromInverter(SmaInverter_t&);
error_e exportToInflux(Influx&, const SmaInverter_t&, unsigned long);

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

    /** Connect to InfluxDB */
    Influx ifx(cfg.influx.host, cfg.influx.port, cfg.influx.org, cfg.influx.bucket, cfg.influx.token, cfg.influx.measurement);
    if (eError_ok != ifx.connectNow()) {
        cerr << "main: InfluxDB connection failed\n";
        return EXIT_FAILURE;
    }

    cout << "Connecting to Inverters...\n";
    std::vector<SmaInverter_t> aInverters;

    /* For each configured inverter */
    for (const InverterConfig& invCfg : cfg.inverters) {
        aInverters.push_back((SmaInverter_t) {
            .networkHandle = network_init(cfg.timeout),
            .unitIdentifier = invCfg.unitIdentifier,
            .name = invCfg.name,
        });

        /* Connect to the inverter */
        auto inv = aInverters.back();
        if (eError_ok != network_connect(inv.networkHandle, invCfg.ip.c_str(), eModbus_port)) {
            cerr << "Couldn't connect to inverter\n";
            return EXIT_FAILURE;
        }
    };

    /* Main loop */
    for (;;) {

        /* Divide by interval to have a rounded timestamp */
        unsigned long currentTimestamp = time(NULL);
        currentTimestamp = currentTimestamp - (currentTimestamp % cfg.interval);

        for (SmaInverter_t& inv : aInverters) {

            if (getValuesFromInverter(inv) != eError_ok) {
                cerr << "Processing inverter failed " << endl;
                goto ERROR_HANDLER;
            }
        }

        if (cfg.debug) {

            for (SmaInverter_t& inv : aInverters) {
                cout << to_string(inv) << "\n";
            }
        }

        /** Export to InfluxDB using the same timestamp */

        for (SmaInverter_t& inv : aInverters) {
            exportToInflux(ifx, inv, currentTimestamp);
        }

        sleep(cfg.interval);
    }

ERROR_HANDLER:
    for (SmaInverter_t& inv : aInverters) {
        network_close(inv.networkHandle);
        network_deinit(inv.networkHandle);
    }
    ifx.close();

    return EXIT_FAILURE;
}

/**
 * @brief requests everything needed from an inverter and pushes to influxDB
 * @param[in,out] inv		inverter struct with IP already filled in
 * @retval eError_ok		if successfully processed inverter
 * @retval eError_failed	if unsuccessfull to process the given inverter
 */
error_e getValuesFromInverter(SmaInverter_t& inv)
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
    if (eError_ok != fetchU32Multiple(&handle, eSmaModbusRegister_dcCurrentInput1_S32, (eSmaModbusRegister_lineCurrentOnAllLineConductors - eSmaModbusRegister_dcCurrentInput1_S32) / 2, aRegisters)) {
        return eError_failed;
    }

    /** DC current input 1 (A) S32; FIX3 */
    inv.idc1 = ((double)aRegisters[eSmaModbusRegister_dcCurrentInput1_S32 - eSmaModbusRegister_dcCurrentInput1_S32] / 1000);
    /** DC voltage input 1 (V) S32; FIX2 */
    inv.udc1 = ((double)aRegisters[(eSmaModbusRegister_dcVoltageInput1_S32 - eSmaModbusRegister_dcCurrentInput1_S32) / 2] / 100);
    /** DC power input 1 (W) S32; FIX0 */
    inv.pdc1 = aRegisters[(eSmaModbusRegister_dcPowerInput1_S32 - eSmaModbusRegister_dcCurrentInput1_S32) / 2];
    /** Active power of line conductor L1 (W) S32; FIX0 */
    inv.pac1 = aRegisters[(eSmaModbusRegister_activePowerOnallLineConductors - eSmaModbusRegister_dcCurrentInput1_S32) / 2];
    /** Line voltage, line conductor L1 to N (V) U32; FIX2 */
    inv.uac1 = ((double)aRegisters[(eSmaModbusRegister_lineVoltageLineConductorL1ToN - eSmaModbusRegister_dcCurrentInput1_S32) / 2] / 100);
    /** Line current of line conductor L1 (A); S32; FIX3 */
    inv.iac1 = ((double)aRegisters[(eSmaModbusRegister_lineCurrentOnAllLineConductors - eSmaModbusRegister_dcCurrentInput1_S32) / 2] / 1000);

#if 0
    regs = modbus_read_registers(&t, eSmaModbusRegister_powerFrequency, c_registerSize);
    if (regs == NULL) {
        return eError_failed;
    }

    /** Power frequency (Hz) U32; FIX2 */
    inv.GridFreq = ((double)getValue(regs, eSmaModbusRegister_powerFrequency, eSmaModbusRegister_powerFrequency) / 100); // Hz
    /** Reactive power on all line conductors (VAr) S32; FIX0 */
    inv.ReactivePower = getValue(regs, eSmaModbusRegister_powerFrequency, eSmaModbusRegister_reactivePowerOnAllLineConductors); // VAr
    /** Apparent power on all line conductors (VA) S32; FIX0 */
    inv.ApparentPower = getValue(regs, eSmaModbusRegister_powerFrequency, eSmaModbusRegister_apparentPowerOnAllLineConductors); // VA

    modbus_free_registers(regs);
    regs = modbus_read_registers(&t, eSmaModbusRegister_internalTemperature, (eSmaModbusRegister_dcPowerInput2_S32 - eSmaModbusRegister_internalTemperature) + c_registerSize);
    if (regs == NULL) {
        return eError_failed;
    }

    /** Internal temperature (C) S32; TEMP */
    inv.Temperature = getValue(regs, eSmaModbusRegister_internalTemperature, eSmaModbusRegister_internalTemperature) / 10;

    /** DC current input 2 (A); S32; FIX3 */
    inv.Idc2 = ((double)getValue(regs, eSmaModbusRegister_internalTemperature, eSmaModbusRegister_dcCurrentInput2_S32) / 1000);
    /** DC voltage input 2 (V); S32; FIX2 */
    inv.Udc2 = ((double)getValue(regs, eSmaModbusRegister_internalTemperature, eSmaModbusRegister_dcVoltageInput2_S32) / 100);
    /** DC power input 2 (W); S32; FIX0 */
    inv.Pdc2 = getValue(regs, eSmaModbusRegister_internalTemperature, eSmaModbusRegister_dcPowerInput2_S32);

    modbus_free_registers(regs);

#if 0
	/* My inverters don't support a heatsink temperature reading :c */
	regs = modbus_read_registers(&t, eSmaModbusRegister_heatSinkTemperature1, c_registerSize);
	if (regs == NULL)
	{
		return eError_failed;
	}

	/** Heat sink temperature (C) S32; TEMP; 34109 */
	inv.HeatsinkTemperature = ((double)getValue(regs, eSmaModbusRegister_heatSinkTemperature1, eSmaModbusRegister_heatSinkTemperature1) / 10);

	modbus_free_registers(regs);
#endif
#endif
    return eError_ok;
}

/**
 * @brief constructs the line protocol to send to Influx
 * @param ifx
 * @param inv
 * @param currentTimestamp
 * @return error_e from Influx.post()
 */
error_e exportToInflux(Influx& ifx, const SmaInverter_t& inv,
    unsigned long currentTimestamp)
{
    InfluxLine line("inverter");

    line.setTimestamp(currentTimestamp);

    line.addTag("name", inv.name);

    line.addField("status", int(inv.statusOfDevice));
    line.addField("gridConnection", int(inv.gridConnection));

    line.addField("dayYield", inv.dayYield);
    line.addField("totalYield", inv.totalYield);

    /* Inverter's grid relay contactor is not closed -> Post only limited values, should find some other way*/
    if (inv.gridConnection != eSmaModbusPvSystemUtilityGridConnection_utilityGrid) {
        return ifx.post(line.getLine());
    } else {
        line.addField("temperature", inv.temperature);

        line.addField("gridFreq", inv.gridFreq);

        line.addField("pac1", inv.pac1);
        line.addField("pdc1", inv.pdc1);
        line.addField("pdc2", inv.pdc2);

        line.addField("uac1", inv.uac1);
        line.addField("udc1", inv.udc1);
        line.addField("udc2", inv.udc2);

        line.addField("iac1", inv.iac1);
        line.addField("idc1", inv.idc1);
        line.addField("idc2", inv.idc2);

        line.addField("reactivePower", inv.reactivePower);
        line.addField("apparentPower", inv.apparentPower);

        return ifx.post(line.getLine());
    }
    return eError_ok;
}
