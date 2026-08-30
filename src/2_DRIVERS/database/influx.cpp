/**
 * @file influx.cpp
 * @brief InfluxDB LineProtocol class implementation
 */
#include <curl/curl.h>
#include <curl/easy.h>
#include <iostream>
#include <cassert>
#include <cstring> // memset
#include <string>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>

#include "influx.hpp"
#include "error_codes.h"

/** Global */
extern bool g_debug;

namespace influx {

/** Constants */
static constexpr long c_influxOk = 204;

/** Static */
const unsigned int Influx::s_bufsize = 8196;

/** Constructor */
Influx::Influx(const std::string &host, const unsigned short port, const std::string &org, const std::string &bucket, const std::string &token, const std::string &measurement)
{
    m_curl = curl_easy_init();

    /* Configure Curl Host -> POST /api/v2/write?bucket=%s&org=%s&precision=s*/
    /** @warning curl guesses the scheme unless we explicitely pass it */
    const std::string c_url = host + ':' + std::to_string(port) + 
        std::string("/api/v2/write?bucket=") + bucket + 
        std::string("&org=") + org + 
        std::string("&precision=s");
    std::cout << "URL: " << c_url << "\n";
    curl_easy_setopt(m_curl, CURLOPT_URL, c_url.c_str());

    /* Configure Content-Type: text/plain */
    m_curl_headers = curl_slist_append(NULL, "Content-Type: text/plain");

    /* Authorization: Token */
    std::string authorizationStr  = std::string("Authorization: Token ") + token;
    m_curl_headers = curl_slist_append(m_curl_headers, authorizationStr.c_str());

    /* Apply header config */
    curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_curl_headers);

    /* Set timeout */
    curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 1);
}

/** Destructor - Cleanup */
Influx::~Influx()
{
    curl_easy_cleanup(m_curl);
    curl_slist_free_all(m_curl_headers);
}

/**
 * @brief posts the measurment
 * @retval eError_invalid   if curl wasn't initialized correctly
 * @retval eError_failed    if curl wasn't able to perform the transaction
 * @retval eError_ok        if successfull
 */
error_e Influx::post(const std::string &data)
{
    using namespace std;

    if (nullptr == m_curl){
        return eError_invalid;
    }

    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data.c_str());

    const CURLcode ret = curl_easy_perform(m_curl);
    if (CURLE_OPERATION_TIMEDOUT == ret) {
        return eError_timeout;
    }

    if (CURLE_OK == ret) {
        /* Get response code */
        long http_code = 0;
        curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &http_code);

        if (c_influxOk == http_code) {
            return eError_ok;
        }

        printf("HTTP Code: %ld\n", http_code);
    }

    return eError_failed;
}


error_e init(void)
{
    return CURLE_OK == curl_global_init(0) ? eError_ok : eError_failed;
}

error_e deinit(void)
{
    curl_global_cleanup();

    return eError_ok;
}

};