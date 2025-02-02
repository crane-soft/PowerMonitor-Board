#ifndef DS18B20scan_h
#define DS18B20scan_h
#include "DS18B20.h"

#define MAX_DS18B20 3

class DS18B20_scan
{
public:    
    DS18B20_scan();
    int EnumDevices();
    void StartConversions();
    void ReadTemperatures();
    int TemperatureValue (int device);
    void getTemperatureData (int16_t * data);
    bool ScanDevices();

private:    
    int device_cnt;
    int scan_status;
    int conversion_time;
    OneWire  ds;
    DS18B20 ds_devices[MAX_DS18B20];
};

#endif // DS18B20_h
