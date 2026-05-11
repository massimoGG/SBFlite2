#ifndef __INFLUX_HPP__
#define __INFLUX_HPP__

#include <vector>
#include <optional>
#include <string>

#include <error_codes.h>
#include "influxline.hpp"

class Influx
{
public:
    /* Delete default constructor */
    Influx() = delete;

    explicit Influx(const std::string &host, const unsigned short port, const std::string &org, const std::string &bucket, const std::string &token, const std::string &measurement);

    error_e connectNow(void);
    error_e close(void);
    error_e post(const std::string &body);

    const std::string &getMeasurement(void) const;

private:
    /** The HTTP buffer size */
    static const unsigned int s_bufsize;

    /** Our socket file descriptor */
    std::optional<int> m_sockfd;

    const std::string m_host;
    const unsigned short m_port;
    const std::string m_org;
    const std::string m_bucket;
    const std::string m_token;
    const std::string m_measurement;
};

#endif
