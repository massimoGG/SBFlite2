#ifndef __INFLUX_HPP__
#define __INFLUX_HPP__

#include <string>

#include <error_codes.h>
#include <curl/curl.h>

namespace influx {

class Influx
{
public:
    /* Delete default constructor */
    Influx() = delete;

    explicit Influx(const std::string &host, const unsigned short port, const std::string &org, const std::string &bucket, const std::string &token, const std::string &measurement);

    ~Influx();

    error_e post(const std::string &data);

private:
    /** The HTTP buffer size */
    static const unsigned int s_bufsize;

    CURL *m_curl;
    struct curl_slist *m_curl_headers;
};

error_e init(void);
error_e deinit(void);

};

#endif
