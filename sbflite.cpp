#include <iostream>
#include <vector>

#include "sbflite.h"
#include "Inverter.h"
#include "Ethernet.h"
#include "InfluxDB.h"
#include "types.h"

int main(int argc, char *argv[])
{
    /**
     * Get list of fixed IPs
     * Create that many inverter objects
     * iterate through -> inverter.process();
     * inverter.getInverterData() ? and then store to database?
     */
    std::vector<std::string> ipaddresses;
    ipaddresses.push_back("192.168.1.51");
    ipaddresses.push_back("192.168.1.52");

    int rc = 0;
    printf("Connecting to Local Network...\n");

    // Setup UDP socket
    rc = ethConnect();
    if (rc != 0) {
        std::cerr << "Failed to set up socket connection." << std::endl;
        return rc;
    }

    for (const std::string &ip : ipaddresses) {
        Inverter inverter(ip);
        inverter.process();
    }

    return 0;
}
