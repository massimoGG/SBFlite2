#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <vector>
#include <string>
#include <error_codes.hpp>

/** @brief Influx configuration */
struct InfluxConfig
{
    /** @brief Bool indicating whether we're using InfluxDB */
    bool enabled;

    /** @brief The Influx host address */
    std::string host;

    /** @brief The Influx port */
    unsigned short port;

    /** @brief The Influx organisation */
    std::string org;

    /** @brief The Influx bucket */
    std::string bucket;

    /** @brief The Influx token */
    std::string token;
};

/** @brief Config for one inverter */
struct InverterConfig
{
    /** @brief The IP address */
    std::string ip;

    /** @brief The name the user gave this inverter */
    std::string name;
};

/** @brief General Application configuration */
struct Configuration
{
    /** @brief Should we do debug logs */
    bool debug;

    /** @brief Instance name */
    std::string instanceName;

    /** @brief The interval between fetches */
    unsigned interval;

    /** @brief Influx configuration */
    InfluxConfig influx;

    /** @brief Inverters configuration */
    std::vector<InverterConfig> inverters;
};

error_e getConfiguration(Configuration &cfg);

#endif