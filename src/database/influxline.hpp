/**
 * @file line.hpp
 * @brief InfluxDB LineProtocol class
 */
#ifndef __INFLUXLINE_HPP__
#define __INFLUXLINE_HPP__

#include <string>
#include <vector>
#include <utility>

/** @brief Influx LineProtocol wrapper */
class InfluxLine
{
public:
    explicit InfluxLine(const std::string &name);

    InfluxLine &addTag(const std::string tagKey, const std::string tagValue);

    template <typename T>
    InfluxLine &addField(const std::string fieldKey, const T fieldValue)
    {
        /* Construct key/value pair */
        m_fields.push_back(fieldKey + "=" + std::to_string(fieldValue));
        return *this;
    }

    /* Specialisation for int */
    InfluxLine &addField(const std::string fieldKey, const int fieldValue)
    {
        /* Construct key/value pair */
        m_fields.push_back(fieldKey + "=" + std::to_string(fieldValue) + 'i');
        return *this;
    }

    InfluxLine &setTimestamp(const unsigned long long time);

    std::string getLine(void);

private:
    using kv = std::pair<std::string, std::string>;

    std::string getTags(void);
    std::string getFields(void);

    /** The current measurement's name */
    std::string m_measurement;

    /** All tags */
    std::vector<kv> m_tags;

    /** All fields */
    std::vector<std::string> m_fields;

    /* Current timestamp */
    std::string m_timestamp;
};

#endif