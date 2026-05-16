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
            .Name = invCfg.name,
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

    if (eError_ok != fetchU32(&handle, eSmaModbusRegister_statusOfDevice, &value)) {
        return eError_failed;
    }
    inv.statusOfDevice = static_cast<smaModbus_statusOfTheDevice_e>(value);

#if 0
    
    regs = modbus_read_registers(&t, eSmaModbusRegister_utilityGridContactor, c_registerSize);
    if (regs == NULL) {
        return eError_failed;
    }

    inv.GridRelay = static_cast<smaModbus_utilityGridContactor_e>(getValue(regs, eSmaModbusRegister_utilityGridContactor, eSmaModbusRegister_utilityGridContactor));

    modbus_free_registers(regs);
    regs = modbus_read_registers(&t, eSmaModbusRegister_totalAcEnergyFedInOnAllLineConductors_Wh,
        /* Offset in bytes + the last register */
        (eSmaModbusRegister_energyFedInOnTheCurrentDayOnAllLineConductors_Wh - eSmaModbusRegister_totalAcEnergyFedInOnAllLineConductors_Wh) + c_registerSize);
    if (regs == NULL) {
        return eError_failed;
    }

    /* Total Yield (Wh) U32; FIX0 */
    inv.TotalYield = getValue(regs, eSmaModbusRegister_totalAcEnergyFedInOnAllLineConductors_Wh, eSmaModbusRegister_totalAcEnergyFedInOnAllLineConductors_Wh);
    /* Energy fed in on the current day on all line conductors (Wh) U32; FIX0 */
    inv.DayYield = getValue(regs, eSmaModbusRegister_totalAcEnergyFedInOnAllLineConductors_Wh, eSmaModbusRegister_energyFedInOnTheCurrentDayOnAllLineConductors_Wh);

    modbus_free_registers(regs);
    regs = modbus_read_registers(&t, eSmaModbusRegister_dcCurrentInput1_S32,
        /* Offset + last register */
        (eSmaModbusRegister_lineCurrentOnAllLineConductors - eSmaModbusRegister_dcCurrentInput1_S32) + c_registerSize);
    if (regs == NULL) {
        return eError_failed;
    }

    /** DC current input 1 (A) S32; FIX3 */
    inv.Idc1 = ((double)getValue(regs, eSmaModbusRegister_dcCurrentInput1_S32, eSmaModbusRegister_dcCurrentInput1_S32) / 1000);
    /** DC voltage input 1 (V) S32; FIX2 */
    inv.Udc1 = ((double)getValue(regs, eSmaModbusRegister_dcCurrentInput1_S32, eSmaModbusRegister_dcVoltageInput1_S32) / 100);
    /** DC power input 1 (W) S32; FIX0 */
    inv.Pdc1 = getValue(regs, eSmaModbusRegister_dcCurrentInput1_S32, eSmaModbusRegister_dcPowerInput1_S32);
    /** Active power of line conductor L1 (W) S32; FIX0 */
    inv.Pac1 = getValue(regs, eSmaModbusRegister_dcCurrentInput1_S32, eSmaModbusRegister_activePowerOnallLineConductors);
    /** Line voltage, line conductor L1 to N (V) U32; FIX2 */
    inv.Uac1 = ((double)getValue(regs, eSmaModbusRegister_dcCurrentInput1_S32, eSmaModbusRegister_lineVoltageLineConductorL1ToN) / 100);
    /** Line current of line conductor L1 (A); S32; FIX3 */
    inv.Iac1 = ((double)getValue(regs, eSmaModbusRegister_dcCurrentInput1_S32, eSmaModbusRegister_lineCurrentOnAllLineConductors) / 1000);

    modbus_free_registers(regs);
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

    line.addTag("name", inv.Name);

    line.addField("Condition", int(inv.Condition));

    line.addField("DayYield", inv.DayYield);
    line.addField("TotalYield", inv.TotalYield);

    /* Inverter's grid relay contactor is not closed -> Post only limited values, should find some other way*/
    if (inv.GridRelay != eSmaModbusUtilityGridContactor_closed) {
        return ifx.post(line.getLine());
    } else {
        line.addField("GridRelay", int(inv.GridRelay));
        line.addField("Temperature", inv.Temperature);

        line.addField("GridFreq", inv.GridFreq);

        line.addField("Pac1", inv.Pac1);
        line.addField("Pdc1", inv.Pdc1);
        line.addField("Pdc2", inv.Pdc2);

        line.addField("Uac1", inv.Uac1);
        line.addField("Udc1", inv.Udc1);
        line.addField("Udc2", inv.Udc2);

        line.addField("Iac1", inv.Iac1);
        line.addField("Idc1", inv.Idc1);
        line.addField("Idc2", inv.Idc2);

        line.addField("ReactivePower", inv.ReactivePower);
        line.addField("ApparentPower", inv.ApparentPower);

        return ifx.post(line.getLine());
    }
    return eError_ok;
}
