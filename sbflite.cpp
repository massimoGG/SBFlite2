#include <iostream>

#include "sbflite.h"


int main(int argc, char *argv[])
{
    /**
     * Get list of fixed IPs
     * Create that many inverter objects
     * iterate through -> inverter.process();
     * inverter.getInverterData() ? and then store to database?
     */
    char ip[] = "192.168.1.51";
    // char ip[] = "192.168.1.52"
    // Maybe use Vectors?
    // Set up network

    Inverter inverter(ip);
    inverter.process();

    return 0;
}

