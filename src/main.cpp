/** Includes */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <array>

#include "error_codes.hpp"
#include "influx.hpp"
#include "modbus.hpp"
#include "inverter.hpp"

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
error_e processInverter(SMA_Inverter &, modbus_t &);
error_e exportToInflux(Influx &, const SMA_Inverter &, unsigned long);

/** Public functions */

int main(int argc, char *argv[])
{
	using namespace std;

	InfluxConfig influxCfg;

	if (eError_ok != processEnvVariables(influxCfg))
	{
		exit(EXIT_FAILURE);
	}

	/**
	 * Connect to InfluxDB
	 */
	Influx ifx(influxCfg.host, influxCfg.port, influxCfg.org, influxCfg.bucket, influxCfg.token);
	if (ifx.connectNow() != 0)
	{
		cerr << "main: InfluxDB connection failed\n";
		return -1;
	}

	cout << "Connecting to Inverters...\n";

	/** Inverters */
	std::array<SMA_Inverter, 2> aInverters{
		(SMA_Inverter){
			.Ip = strdup("172.19.30.0"),
			.Port = 502,
			.Name = strdup("SB3000TL-21")},
		(SMA_Inverter){
			.Ip = strdup("172.19.40.0"),
			.Port = 502,
			.Name = strdup("SB4000TL-21")},
	};

	// Connect to clients
	modbus_t *sb3000_conn = modbus_connect_tcp(sb3000.Ip, sb3000.Port);
	cout << "Connected to SB3000TL";

	SMA_Inverter sb4000 =
		modbus_t *sb4000_conn = modbus_connect_tcp(sb4000.Ip, sb4000.Port);
	cout << "Connected to SB4000TL";

	for (;;)
	{
		for (auto const &inv : aInverters)
		{
			sb3000_conn
		}
	}

	for (unsigned long long i = 0;; i++)
	{
		unsigned long currentTimestamp = time(NULL);

		processInverter(sb3000, *sb3000_conn);
		processInverter(sb4000, *sb4000_conn);

		if (s_debug)
		{
			cout << to_string(sb3000) << "\n";
			cout << to_string(sb4000) << "\n";
		}

		/** Export to InfluxDB using the same timestamp */
		int ret = exportToInflux(ifx, &sb3000, currentTimestamp);
		ret = exportToInflux(ifx, &sb4000, currentTimestamp);
		if (ret != 0)
		{
			break;
			// Abort if connection with Influx lost
		}

		sleep(s_interval);
	}

	modbus_close(sb3000_conn);
	modbus_close(sb4000_conn);

	return 0;
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
		cerr << "Missing required environment variables";

		return eError_failed;
	}

	const char *pPort = getenv("INFLUX_PORT");
	if (nullptr == pPort)
	{
		/* Default HTTP port*/
		cfg.port = 80;
	}
	else
	{
		cfg.port = atoi(pPort);
	}

	const char *pPort = getenv("INFLUX_PORT");
	if (nullptr == pPort)
	{
		/* Default HTTP port*/
		cfg.port = 80;
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
error_e processInverter(SMA_Inverter &inv, modbus_t &t)
{
	modbus_regs regs;

	t.slave = 0x03; // 0 = broadcast, 3= my inverters

	/**
	 * Inverter Condition
	 * 	35: Fault (Alm)
	 *  303: Off (Off)
	 *  307: Ok (Ok)
	 *  455: Warning (Wrn)
	 * */
	regs = modbus_read_registers(&t, 30201, 4);
	if (regs == NULL)
	{
		return eError_failed;
	}

	inv.Condition = getValue(regs, 30201, 30201);

	modbus_free_registers(regs);

	/**
	 * Grid Relay
	 * 	51: Closed (Cls)
	 *  311: Open (Opn)
	 *  16777213: Information not available (NaNStt)
	 */
	regs = modbus_read_registers(&t, 30211, 16);
	if (regs == NULL)
	{
		return eError_failed;
	}
	inv.GridRelay = getValue(regs, 30211, 30217);

	modbus_free_registers(regs);

	/**
	 * Total Yield and Day Yield
	 */
	regs = modbus_read_registers(&t, 30529, 54);
	if (regs == NULL)
	{
		return eError_failed;
	}

	inv.TotalYield = getValue(regs, 30529, 30529);
	inv.DayYield = getValue(regs, 30529, 30535);

	modbus_free_registers(regs);

	/**
	 * DC AMP, VOLT, WATT A; AC Watt, L1-3, ACVOLTAGE L1-3
	 * Grid freq, AC_R_POWER_L1-3, AC_A_POWER_L!-3
	 */
	regs = modbus_read_registers(&t, 30769, 52);
	if (regs == NULL)
	{
		return eError_failed;
	}
	inv.Udc1 = ((double)getValue(regs, 30769, 30771) / 100);
	inv.Idc1 = ((double)getValue(regs, 30769, 30769) / 1000);
	inv.Pdc1 = getValue(regs, 30769, 30773);

	inv.Uac1 = ((double)getValue(regs, 30769, 30783) / 100);
	inv.Pac1 = getValue(regs, 30769, 30775);

	modbus_free_registers(regs);

	/**
	 * Grid Freq, Reactive Power, Apparent Power
	 */
	regs = modbus_read_registers(&t, 30803, 10);
	if (regs == NULL)
	{
		return eError_failed;
	}

	inv.GridFreq = ((double)getValue(regs, 30803, 30803) / 100); // Hz
	inv.ReactivePower = getValue(regs, 30803, 30805);			 // VAr
	inv.ApparentPower = getValue(regs, 30803, 30813);			 // VA

	modbus_free_registers(regs);

	/**
	 * TEMPERATURE, DC AMP, VOLT, WATT B AMP_L1-3
	 */
	regs = modbus_read_registers(&t, 30953, 30);
	if (regs == NULL)
	{
		return eError_failed;
	}

	inv.Temperature = getValue(regs, 30953, 30953) / 10;

	inv.Udc2 = ((double)getValue(regs, 30953, 30959) / 100);
	inv.Idc2 = ((double)getValue(regs, 30953, 30957) / 1000);
	inv.Pdc2 = getValue(regs, 30953, 30961);

	inv.Iac1 = ((double)getValue(regs, 30953, 30977) / 1000);

	modbus_free_registers(regs);

	return eError_ok;
}

error_e exportToInflux(Influx &ifx, const SMA_Inverter &inv,
					   unsigned long currentTimestamp)
{
	ifx.clear();

	// can be a way to see if the inverter is off?
	if (inv->Temperature > 10000)
	{
		return ifx.meas("measurement")
			.tag("inverter", inv->Name)
			.field("Condition", inv->Condition)

			// .field("Heatsink", inv->HeatsinkTemperature)
			.field("DayYield", inv->DayYield)
			.field("TotalYield", inv->TotalYield)

			.field("GridRelay", inv->GridRelay)
			.field("GridFreq", inv->GridFreq)

			.timestamp(currentTimestamp)
			.post();
	}

	return ifx.meas("measurement")
		.tag("inverter", inv->Name)
		.field("Condition", inv->Condition)

		.field("Temperature", inv->Temperature)
		// .field("Heatsink", inv->HeatsinkTemperature)
		.field("DayYield", inv->DayYield)
		.field("TotalYield", inv->TotalYield)

		.field("Pac1", inv->Pac1)
		.field("Pdc1", inv->Pdc1)
		.field("Pdc2", inv->Pdc2)

		.field("Uac1", inv->Uac1)
		.field("Udc1", inv->Udc1)
		.field("Udc2", inv->Udc2)

		.field("Iac1", inv->Iac1)
		.field("Idc1", inv->Idc1)
		.field("Idc2", inv->Idc2)

		.field("GridRelay", inv->GridRelay)
		.field("GridFreq", inv->GridFreq)
		.field("ReactivePower", inv->ReactivePower)
		.field("ApparentPower", inv->ApparentPower)

		.timestamp(currentTimestamp)
		.post();
}
