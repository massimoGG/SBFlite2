#ifndef __influxdb_h_
#define __influxdb_h_

#include <cstring>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>

#include <map>
#include <iostream>
#include <vector>
#include <optional>
#include "error_codes.hpp"
#include "line.hpp"

class Influx
{
public:
    /* Delete default constructor */
    Influx() = delete;

    explicit Influx(const std::string &host, const unsigned short port, const std::string &org, const std::string &bucket, const std::string &token);

    error_e connectNow(void);
    error_e close(void);
    error_e post(void);

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

    InfluxLine m_line;
};

#endif
