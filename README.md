### **Introduction**
`SBFlite` is an open source SCADA system written in C/C++ to fetch instantaneous values from [SMA® Sunny Boy](https://www.sma.de/en/products/solarinverters/sunny-boy-30-36-40-50-60) inverters and publish these values to an [Influx](https://docs.influxdata.com/influxdb/v2/) database.

### **What it does**
This program connects over [Modbus](https://www.modbus.org/) to an inverter and reads its instantaneous power generation data. The collected data is then exported to an Influx database.

It has been continuously (+5 years as of writing this) tested on
- SMA Sunny Boy 3000 with the webconnect module (SB3000TL-21)
- SMA Sunny Boy 4000 with the webconnect module (SB4000TL-21)

### **Modbus**
> [!warning]
> This application uses the webconnect's Modbus protocol. Enable the Modbus TCP service using the [Sunny explorer](https://www.sma.de/en/products/energy-management/sunny-explorer) application.
> Installer credentials might be needed for this.

## Output example
The following is a debug output of the program.
```
---------------------------
INVERTER - SB4000TL-21
172.19.40.0
---------------------------
Total yield: 39230580Wh
Day yield: 1954Wh
Inverter
        Temperature: 30.000000C Heatsink: 0.000000C
DC 1
        Volt: 358.760000V
        Amp: 0.936000A
        Watt: 335W
DC 2
        Volt: 220.240000V
        Amp: 0.847000A
        Watt: 186W
AC
        Volt: 233.830000V
        Amp: 2.147000A
        Watt: 496W
        GridFreq: 49.990000
        ReactiveP: 0 VAr
        ApparentP: 0
```

These values are sent to an Influx database 2.0 using the HTTP line protocol.

```
INFLUXDB DEBUG: POST /api/v2/write?bucket=solar&org=massimogg&precision=s HTTP/1.1
Host: 172.17.3.0:8086
User-Agent: influxdb-client-cheader
Content-Length: 303
Authorization: Token [token]

inverter,name=SB4000TL-21 Condition=307i,Temperature=30.000000,DayYield=1954.000000,TotalYield=39230580.000000,Pac1=496.000000,Pdc1=335.000000,Pdc2=186.000000,Uac1=233.830000,Udc1=358.760000,Udc2=220.240000,Iac1=2.147000,Idc1=0.936000,Idc2=0.847000,GridRelay=51i,GridFreq=49.990000,ReactivePower=0.000000,ApparentPower=0.000000 1716631685
```

# Build instructions

> [!Note]
> Clone this repository using the following command `git clone <url> --recurse-submodules` or if you already pulled, initialize the submodules with `git submodule update --init --recursive`

## Docker
Simply install docker on your host, configure the TOML configuration file and run `docker compose up -d`

## Binary
1. Required Linux packages:
- gcc
- cmake
2. Compile the project with
```
cmake -B build && make -j
```
3. Run (or create a systemd configuration file pointing to) ./build/SBFlite 

### **Known bugs and limitations**
For a list of known bugs, consult the [issues](https://github.com/massimoGG/SBFlite2/issues). If you find a bug, please create an [issue](https://github.com/massimoGG/SBFlite2/issues).  


