#include "influxline.hpp"

/**
 * @brief creates a new measurement (and thus line)
 * @param measurement name of the measurement
 */
InfluxLine::InfluxLine(const std::string &measurement) : m_measurement{measurement}, m_tags{}, m_fields{}, m_timestamp{}
{
}

/**
 * @brief add a tag key/value pair
 * @param tagKey
 * @param tagValue
 * @return InfluxLine&
 */
InfluxLine &InfluxLine::addTag(const std::string tagKey, const std::string tagValue)
{
    m_tags.push_back({tagKey, tagValue});
    return *this;
}

/**
 * @brief gets the tags as a formatted LineProtocol line
 * @return std::string
 */
std::string InfluxLine::getTags(void)
{
    std::string ret{};

    for (const auto &kv : m_tags)
    {
        ret += "," + kv.first + "=" + kv.second + " ";
    }

    return ret;
}

/**
 * @brief gets the fields as a formatted LineProtocol line
 * @return std::string
 */
std::string InfluxLine::getFields(void)
{
    std::string ret{};

    /* Construct m_fields section */
    for (size_t i = 0; i < m_fields.size(); i++)
    {
        ret += m_fields[i];

        /* Append comma between key/value except for the last pair */
        if (i + 1 < m_fields.size())
        {
            ret += ",";
        }
    }

    return ret;
}

/**
 * @brief set the timestamp of the measurement
 * @param time
 * @return InfluxLine&
 */
InfluxLine &InfluxLine::setTimestamp(const unsigned long long time)
{
    m_timestamp = std::to_string(time);

    return *this;
}

/**
 * @brief constructs the LineProtocol line
 * @return const std::string&
 */
std::string InfluxLine::getLine(void)
{
    std::string ret{};

    /* Begin with  Measurement */
    ret += m_measurement;

    /* Append tags if any set */
    if (m_tags.size())
    {
        ret += "," + getTags() + " ";
    }

    /* Append fields */
    ret += getFields();

    /* Append timestamp */
    ret += " " + m_timestamp;

    return ret;
}
