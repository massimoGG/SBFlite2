#ifndef __INFLUX_HPP__
#define __INFLUX_HPP__

#include <vector>
#include <optional>
#include <string>

#include <error_codes.hpp>
#include "influxline.hpp"

class Influx
{
public:
    /* Delete default constructor */
    Influx() = delete;

    explicit Influx(const std::string &host, const unsigned short port, const std::string &org, const std::string &bucket, const std::string &token);

    error_e connectNow(void);
    error_e close(void);
    error_e post(const std::string &body);

private:
    /** The HTTP buffer size */
    static const unsigned int s_bufsize;

    /** Our socket file descriptor */
    std::optional<int> m_sockfd;

    std::string m_host;
    unsigned short m_port;
    std::string m_org;
    std::string m_bucket;
    std::string m_token;
};

#endif
