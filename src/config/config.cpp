#include <iostream>
#include "config.hpp"
#include <toml.hpp>
#include <cassert>

#define TOML_FILENAME "/etc/sbflite/config.toml"

/**
 * @brief Get the Configuration from the TOML file
 * @param cfg
 * @retval eError_ok        Successfully read config
 * @retval eError_failed    Failed to read the config file
 */
error_e getConfiguration(Configuration &cfg)
{
    using namespace std;

    /* Read TOML file */
    const auto input = toml::try_parse(TOML_FILENAME);

    if (input.is_err())
    {
        cerr << input.unwrap_err().at(0) << endl;
        return eError_failed;
    }

    const auto data = input.unwrap();

    /* Read general configuration */
    assert(data.at("general").is_table());
    cfg.debug = toml::find_or<bool>(data, "general", "debug", false);
    cfg.interval = toml::find_or<unsigned>(data, "general", "interval", 10);
    cfg.instanceName = toml::find_or<string>(data, "general", "instance", "SBFlite");

    if (cfg.debug)
    {
        cout << "Interval: " << cfg.interval << endl;
        cout << "Instance name: " << cfg.instanceName << endl;
    }

    /* Read Influx configuration */
    assert(data.at("influx").is_table());

    bool influxEnabled = toml::find_or<bool>(data, "influx", "enabled", false);
    if (influxEnabled)
    {
        cfg.influx = {
            .enabled = true,
            .host = toml::find<string>(data, "influx", "host"),
            .port = toml::find_or<unsigned short>(data, "influx", "port", 8086),
            .org = toml::find<string>(data, "influx", "org"),
            .bucket = toml::find<string>(data, "influx", "bucket"),
            .token = toml::find<string>(data, "influx", "token"),
        };
    }
    else
    {
        cfg.influx = {
            .enabled = false,
        };
    }

    /* Read inverters */
    assert(data.at("inverters").is_table());
    const auto &inverterConfigTable = data.at("inverters").as_table();

    auto parseInverter = [](const toml::value &v)
    {
        return (InverterConfig){
            .ip = toml::find<std::string>(v, "ip"),
            .name = toml::find<std::string>(v, "name"),
        };
    };

    for (const auto &inverterConfig : inverterConfigTable)
    {
        cfg.inverters.push_back(parseInverter(inverterConfig.second));
    }

    return eError_ok;
}