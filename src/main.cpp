/** Includes */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <array>
#include <unistd.h> // for sleep()

#include <error_codes.hpp>

#include <database/influx.hpp>
#include <database/influxline.hpp>
#include <inverter/inverter.hpp>
#include <modbus/modbus.hpp>
#include <modbus/modbus_sma.h>

/** Local Typedefs */
struct InfluxConfig
{
	/** String to host */
	char *host;
	/** Organisation */
	char *org;
	/** Bucket */
	char *bucket;
	/** Access token */
	char *token;

	/** HTTP port */
	int port;
};

/** Local variables */
/* Should we print debug logs */
static bool s_debug = false;

/* How many seconds should we wait between requests */
static unsigned int s_interval = 10;

/** Prototypes */
error_e processEnvVariables(InfluxConfig &);
error_e processInverter(SmaInverter_t &, modbus_t &);
error_e exportToInflux(Influx &, const SmaInverter_t &, unsigned long);

/** Public functions */

/**
 * @brief main function
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[])
{
	using namespace std;

	/* The current InfluxDB config */
	InfluxConfig influxCfg;

	if (eError_ok != processEnvVariables(influxCfg))
	{
		exit(EXIT_FAILURE);
	}

	/** Connect to InfluxDB */
	Influx ifx(influxCfg.host, influxCfg.port, influxCfg.org, influxCfg.bucket, influxCfg.token);
	if (eError_ok != ifx.connectNow())
	{
		cerr << "main: InfluxDB connection failed\n";
		return EXIT_FAILURE;
	}

	cout << "Connecting to Inverters...\n";

	/**
	 * @note the following is legacy code. It will all be reimplemented as the following
	 * modbus class
	 * SmaInverter class(Accepting a Modbus interface for communication with inverter)
	 */
	/** Inverters */
	std::array<SmaInverter_t, 2> aInverters{
		(SmaInverter_t){
			.Ip = strdup("172.19.30.0"),
			.Port = eModbus_port,
			.Name = strdup("SB3000TL-21")},
		(SmaInverter_t){
			.Ip = strdup("172.19.40.0"),
			.Port = eModbus_port,
			.Name = strdup("SB4000TL-21")},
	};

	/* This is all temporary */
	std::array<modbus_t *, 2> aModbusConnections{
		modbus_connect_tcp(aInverters[0].Ip.c_str(), aInverters[0].Port),
		modbus_connect_tcp(aInverters[1].Ip.c_str(), aInverters[1].Port),
	};

	for (;;)
	{
		error_e ret = eError_ok;

		unsigned long currentTimestamp = time(NULL);

		processInverter(aInverters[0], *aModbusConnections[0]);
		processInverter(aInverters[1], *aModbusConnections[1]);

		if (s_debug)
		{
			cout << to_string(aInverters[0]) << "\n";
			cout << to_string(aInverters[1]) << "\n";
		}

		/** Export to InfluxDB using the same timestamp */
		ret = exportToInflux(ifx, aInverters[0], currentTimestamp);
		ret = exportToInflux(ifx, aInverters[1], currentTimestamp);
		if (eError_ok != ret)
		{
			cerr << "Error while exporting data to Influx\n";
			break;
			// Abort if connection with Influx lost
		}

		sleep(s_interval);
	}

#if 0
/* should move to the destructor of the modbus unique ptr*/
	modbus_close(sb3000_conn);
	modbus_close(sb4000_conn);
#endif

	return EXIT_SUCCESS;
}

/**
 * @brief gets all required environment variables or fatals out
 * @param cfg               influx config to update with values
 * @retval eError_ok        if successfully fetched all required variables
 * @retval eError_failed    if missing required variables
 */
error_e processEnvVariables(InfluxConfig &cfg)
{
	using namespace std;

	cfg = {
		.host = getenv("INFLUX_HOST"),
		.org = getenv("INFLUX_ORGANISATION"),
		.bucket = getenv("INFLUX_BUCKET"),
		.token = getenv("INFLUX_TOKEN"),
	};

	if ((nullptr == cfg.host) || (nullptr == cfg.org) ||
		(nullptr == cfg.bucket) || (nullptr == cfg.token))
	{
		cerr << "Missing required environment variables\n";

		return eError_failed;
	}

	const char *pPort = getenv("INFLUX_PORT");
	if (nullptr == pPort)
	{
		/* Default Influx port*/
		cfg.port = 8086;
	}
	else
	{
		cfg.port = atoi(pPort);
	}

	const char *pInterval = getenv("INTERVAL");
	if (nullptr != pInterval)
	{
		s_interval = atoi(pInterval);
	}

	/* If the debug flag is set */
	if (nullptr != getenv("DEBUG"))
	{
		s_debug = true;
	}

	return eError_ok;
}

/**
 * @brief requests everything needed from an inverter and pushes to influxDB
 * @param[in,out] inv				inverter struct with IP already filled in
 * @param[in,out] t
 * @retval eError_ok		if successfully processed inverter
 * @retval eError_failed	if unsuccessfull to process the given inverter
 */
error_e processInverter(SmaInverter_t &inv, modbus_t &t)
{
	modbus_regs regs;

	t.slave = 0x03; // 0 = broadcast, 3= my inverters

	regs = modbus_read_registers(&t, 30201, 4);
	if (regs == NULL)
	{
		return eError_failed;
	}

	inv.Condition = static_cast<smaModbus_statusOfTheDevice_e>(getValue(regs, 30201, 30201));

	modbus_free_registers(regs);

	regs = modbus_read_registers(&t, 30211, 16);
	if (regs == NULL)
	{
		return eError_failed;
	}

	inv.GridRelay = static_cast<smaModbus_utilityGridContactor_e>(getValue(regs, 30211, 30217));

	modbus_free_registers(regs);

	regs = modbus_read_registers(&t, 30529, 54);
	if (regs == NULL)
	{
		return eError_failed;
	}

	/* Total Yield (Wh) U32; FIX0 */
	inv.TotalYield = getValue(regs, 30529, 30529);
	/* Energy fed in on the current day on all line conductors (Wh) U32; FIX0 */
	inv.DayYield = getValue(regs, 30529, 30535);

	modbus_free_registers(regs);

	regs = modbus_read_registers(&t, 30769, 52);
	if (regs == NULL)
	{
		return eError_failed;
	}

	/** DC current input 1 (A) S32; FIX3 */
	inv.Idc1 = ((double)getValue(regs, 30769, 30769) / 1000);
	/** DC voltage input 1 (V) S32; FIX2 */
	inv.Udc1 = ((double)getValue(regs, 30769, 30771) / 100);
	/** DC power input 1 (W) S32; FIX0 */
	inv.Pdc1 = getValue(regs, 30769, 30773);

	/** Line voltage, line conductor L1 to N (V) U32; FIX2 */
	inv.Uac1 = ((double)getValue(regs, 30769, 30783) / 100);
	/** Active power of line conductor L1 (W) S32; FIX0 */
	inv.Pac1 = getValue(regs, 30769, 30775);

	modbus_free_registers(regs);

	regs = modbus_read_registers(&t, 30803, 10);
	if (regs == NULL)
	{
		return eError_failed;
	}

	/** Power frequency (Hz) U32; FIX2 */
	inv.GridFreq = ((double)getValue(regs, 30803, 30803) / 100); // Hz
	/** Reactive power on all line conductors (VAr) S32; FIX0 */
	inv.ReactivePower = getValue(regs, 30803, 30805); // VAr
	/** Apparent power on all line conductors (VA) S32; FIX0 */
	inv.ApparentPower = getValue(regs, 30803, 30813); // VA

	modbus_free_registers(regs);

	regs = modbus_read_registers(&t, 30953, 30);
	if (regs == NULL)
	{
		return eError_failed;
	}

	/** Internal temperature (C) S32; TEMP */
	inv.Temperature = getValue(regs, 30953, 30953) / 10;

	/** Heat sink temperature (C)  */
	inv.HeatsinkTemperature = getValue(regs, 30953, 30953) / 10;

	/** DC current input 2 (A); S32; FIX3 */
	inv.Idc2 = ((double)getValue(regs, 30953, 30957) / 1000);
	/** DC voltage input 2 (V); S32; FIX2 */
	inv.Udc2 = ((double)getValue(regs, 30953, 30959) / 100);
	/** DC power input 2 (W); S32; FIX0 */
	inv.Pdc2 = getValue(regs, 30953, 30961);

	/** Line current of line conductor L1 (A); S32; FIX3 */
	inv.Iac1 = ((double)getValue(regs, 30953, 30977) / 1000);

	modbus_free_registers(regs);

	regs = modbus_read_registers(&t, 34109, 2);
	if (regs == NULL)
	{
		return eError_failed;
	}

	/** Heat sink temperature (C) S32; TEMP; 34109 */
	inv.HeatsinkTemperature = ((double)getValue(regs, 34109, 34109) / 10);

	modbus_free_registers(regs);

	return eError_ok;
}

/**
 * @brief constructs the line protocol to send to Influx
 * @param ifx
 * @param inv
 * @param currentTimestamp
 * @return error_e from Influx.post()
 */
error_e exportToInflux(Influx &ifx, const SmaInverter_t &inv,
					   unsigned long currentTimestamp)
{
	/* Inverter's temperature is some MAX_INT value when it's in standby */
	if (inv.Temperature > 10000)
	{

		/* Create Influx measurement */
		InfluxLine line("inverter");
		line.addTag("name", inv.Name);
		line.addField("Condition", inv.Condition);
		line.addField("Heatsink", inv.HeatsinkTemperature);
		line.addField("DayYield", inv.DayYield);
		line.addField("TotalYield", inv.TotalYield);
		line.addField("GridRelay", inv.GridRelay);
		line.addField("GridFreq", inv.GridFreq);
		line.setTimestamp(currentTimestamp);

		return ifx.post(line.getLine());
	}
	else
	{
		InfluxLine line("inverter");
		line.addTag("name", inv.Name);
		line.addField("Condition", inv.Condition);
		line.addField("Temperature", inv.Temperature);
		line.addField("Heatsink", inv.HeatsinkTemperature);

		line.addField("DayYield", inv.DayYield);
		line.addField("TotalYield", inv.TotalYield);

		line.addField("GridRelay", inv.GridRelay);
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

		line.setTimestamp(currentTimestamp);

		return ifx.post(line.getLine());
	}
}
