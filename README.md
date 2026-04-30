### **Introduction**
`SMA ModBus fetcher` (I am not so creative with names :]) is an open source project written in C/C++ to fetch instantaneous values from SMA Sunny Boy inverters.

### **What it does**
This program connects over Modbus to your SMA® solar inverter and reads  instantaneous power generation data. The collected data is stored in an Influx database.

It has been continuously tested on
- SMA Sunny Boy 3000 with the webconnect module (SB3000TL-21)
- SMA Sunny Boy 4000 with the webconnect module (SB4000TL-21)

### **Known bugs and limitations**
For a list of known bugs, consult the [issues](https://github.com/massimoGG/SMA_MODBUS_REQUESTER/issues). If you find a bug, please create an [issue](https://github.com/massimoGG/SMA_MODBUS_REQUESTER/issues).    

### **Documentation**
Refer to the [Wiki](https://github.com/massimoGG/SMA_MODBUS_REQUESTER/wiki) for documentation and FAQ.

## Output example
The following is a typical output of the program.
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
Authorization: Token jj553uNGBo1rHgTuEjb3D-iZhECzs3i99Ubt4S9xAeoccRolxxBGS-rfVXdO2deokw265_FecKYMif-Fwu4NFA==

measurement,inverter=SB4000TL-21 Condition=307i,Temperature=30.000000,DayYield=1954i,TotalYield=39230580i,Pac1=496i,Pdc1=335i,Pdc2=186i,Uac1=233.830000,Udc1=358.760000,Udc2=220.240000,Iac1=2.147000,Idc1=0.936000,Idc2=0.847000,GridRelay=51i,GridFreq=49.990000,ReactivePower=0i,ApparentPower=0i 1716631685
```

# Build instructions

## Docker
see docker-compose.yml and Dockerfile file.

### environment variables
- INFLUX_HOST=influxdb
- INFLUX_PORT=8086
- INFLUX_ORGANISATION=
- INFLUX_BUCKET=solar
- INFLUX_TOKEN=
- INTERVAL=15
- DEBUG=1

## Binary
`make` and `./main`


