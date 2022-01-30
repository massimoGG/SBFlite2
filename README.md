# SBFlite2
SBFlite Version 2, the successor of SBFlite 1.
It retrieves all spot values from SMA solar inverters and is able to store these values to a MySQL database or InfluxDB version 1 or 2.
This project was made for my use-case, my solar inverters which are connected with ethernet (Using the SpeedWire protocol)

# Features
- Spot Data retrieval
- Export to MySQL DB or InfluxDB (v1 and v2)
- Daemon service (auto-retrieval)

# Compared to SBFspot
This whole project couldn't be done without SBFspot.
All credits goes to them, this is just a heavily modified version which fits my needs.
## Differences
- -No bluetooth support
- -No Archival data
- + Optimized for spot data retrieval
- + InfluxDB
