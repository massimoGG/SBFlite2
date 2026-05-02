/**
 * @file influx.cpp
 * @brief InfluxDB LineProtocol class implementation
 */

#include <iostream>
#include <cstring> // memset
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>

#include "influx.hpp"

/** Static */
const unsigned int Influx::s_bufsize = 8196;

/** Constructor */
Influx::Influx(const std::string &host, const unsigned short port, const std::string &org, const std::string &bucket, const std::string &token)
    : m_host{host}, m_port{port}, m_org{org}, m_bucket{bucket}, m_token{token}
{
    m_sockfd = std::nullopt;
}

/**
 * @brief connects to the remote server using socket
 * @retval eError_ok        if we're connected
 * @retval eError_invalid   if we're not in a state to connect (e.g. already connected)
 * @retval eError_failed    if we couldn't connect
 */
error_e Influx::connectNow(void)
{
    using namespace std;

    cout << "influxdb: Connecting to " << m_host << ":" << m_port << " with organisation " << m_org << " and bucket " << m_bucket << ".\n";

    if (m_sockfd)
    {
        /* Can't allow, must close socket first */
        return eError_invalid;
    }

    m_sockfd = m_sockfd ? m_sockfd : socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockfd <= 0)
    {
        cerr << "influxdb: socket failed\n";
        return eError_failed;
    }

    /* Resolve name */
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(m_host.c_str(), NULL, &hints, &res);
    if (status != 0)
    {
        cerr << "influxdb: getaddrinfo error : " << gai_strerror(status) << "\n";

        if (res != nullptr)
        {
            freeaddrinfo(res);
        }

        return eError_failed;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr;
    serv_addr.sin_port = htons(m_port);

    freeaddrinfo(res);

    if (connect(*m_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cerr << "influxdb: connection failed\n";
        return eError_failed;
    }

    cout << "influxdb: Connected!\n";
    return eError_ok;
}

/**
 * @brief closes a possible socket file descriptor
 * @retval eError_ok
 */
error_e Influx::close(void)
{
    if (m_sockfd)
    {
        ::close(*m_sockfd);

        /* Clear optional */
        m_sockfd.reset();
    }

    return eError_ok;
}

error_e Influx::post(const std::string &body)
{
    char header[512];
    std::string buffer;

    ssize_t len = sprintf(header, "POST /api/v2/write?bucket=%s&org=%s&precision=s HTTP/1.1\r\nHost: %s:%d\r\nUser-Agent: influxdb-client-cheader\r\nContent-Length: %d\r\nAuthorization: Token %s\r\n\r\n",
                          m_bucket.c_str(), m_org.c_str(), m_host.c_str(), m_port, (int)body.length(), m_token.c_str());

    // Combine header and body
    buffer = std::string(header) + body;
    size_t buffer_len = buffer.length();

    int rc = write(*m_sockfd, buffer.c_str(), buffer_len);
    if (rc < len)
    {
        // TODO Do this properly :)
        fprintf(stderr, "influxdb: Could not POST!\n");
        if (rc == 0)
        {
            fprintf(stderr, "influxdb: Lost connection\n");
            // Disconnected
            this->connectNow();
        }
        else
        {
            return eError_failed;
        }
    }

    return eError_ok;
}