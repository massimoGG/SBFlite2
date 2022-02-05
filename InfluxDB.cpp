#include "InfluxDB.h"

InfluxDB::~InfluxDB()
{
    this->close();
}

int InfluxDB::open(const std::string ipaddress, const  unsigned short port, const std::string organisation, const std::string bucket, const std::string token)
{
    struct sockaddr_in serv_addr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket() failed");
        return -1;
    }

    serv_addr.sin_family        = AF_INET;
    serv_addr.sin_addr.s_addr   = inet_addr(ipaddress.c_str());
    serv_addr.sin_port          = htons(port);

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect() failed");
        return -2;
    }

    this->organisation  = organisation;
    this->bucket        = bucket;
    this->token         = token;

    return 0;
}

void InfluxDB::close()
{
    if (sockfd > 0)
        ::close(sockfd);
}


InfluxDB InfluxDB::addMeasurement(const std::string name)
{
    body = name;
    return *this;
}

InfluxDB InfluxDB::addTag(
    const std::string tagKey,
    const std::string tagValue)
{
    body += "," + tagKey + tagValue;
    return *this;
}

InfluxDB InfluxDB::setField(
    const std::string fieldKey,
    const std::string fieldValue)
{
    body += " " + fieldKey + "=" + fieldValue + "\n";
    return *this;
}

InfluxDB InfluxDB::setField(
    const std::string fieldKey,
    const int fieldValue)
{
    body += " " + fieldKey + "=" + std::to_string(fieldValue) + "i\n";
    return *this;
}

InfluxDB InfluxDB::setField(
    const std::string fieldKey,
    const float fieldValue)
{
    body += " " + fieldKey + "=" + std::to_string(fieldValue) + "\n";
    return *this;
}

int InfluxDB::sendMeasurement()
{
    //sprintf(body, "%s %s\n", tags.c_str(), value.c_str());
    std::cout << "Sending Body: " << body << std::endl;

    sprintf(header, "POST /api/v2/write?org=%s&bucket=%s&precision=ns HTTP/1.1\r\nHost: influx:8086\r\nAuthorization: Token %s\r\nContent-Length: %ld\r\n\r\n", organisation.c_str(), bucket.c_str(), token.c_str(), body.length());

    if (write(sockfd, header, strlen(header)) < 0) {
        perror("Header write() failed");
        return -1;
    }

    if (write(sockfd, body.c_str(), body.length()) < 0) {
        perror("Body write() failed");
        return -2;
    }

    int ret = read(sockfd, response, sizeof(response));
    if (ret < 0) {
        perror("read() failed. Connection closed?");
        return -3;
    }
    response[ret] = 0;
    std::cout << "Response: " << response << std::endl;

    // 200 = success, 204 = no content; [9 10 11]
    if (response[9] == 2 || response[10] == 0 || response[11] == 4) {
        // Data was succesfully injected
    }
    /*
     * if (reponse[ret] == 204)
     */

    return 0;
}

