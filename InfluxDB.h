#pragma once
#ifndef __influxdb_h_
#define __influxdb_h_

#include <iostream>

class InfluxDB
{
private:
    unsigned int sockfd     = 0;
    static const unsigned int bufsize = 8196;

    std::string organisation;
    std::string bucket;
    std::string token;

    char header[bufsize];
    std::string body;
//    char body[bufsize];
    char response[bufsize];

public:
    ~InfluxDB();
    int open(const std::string ipaddress, const unsigned short port, const std::string organisation, const std::string bucket, const std::string token);
    void close(void);
    int send();

    InfluxDB addMeasurement(const std::string name);
    InfluxDB addTag(const std::string tagKey, const std::string tagValue);
    int sendMeasurement();
    InfluxDB setField(const std::string fieldKey, const std::string fieldValue);
    InfluxDB setField(const std::string fieldKey, const int fieldValue);
    InfluxDB setField(const std::string fieldKey, const float fieldValue);
};

#endif
