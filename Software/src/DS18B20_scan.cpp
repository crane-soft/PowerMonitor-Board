#include "DS18B20_scan.h"

DS18B20_scan::DS18B20_scan()
{
    device_cnt = 0;
    scan_status = 0;
}

int DS18B20_scan::EnumDevices()
{
    for (int i = 0; i < MAX_DS18B20; ++i) {
        if (ds_devices[i].Search_Device(&ds) == false)
            break;
        ++device_cnt;      
    }
    if (device_cnt > 0)
        scan_status = 1;
    //Serial.print ("dev_cnt ");
    //Serial.println (device_cnt);
    return device_cnt;
}

void DS18B20_scan::StartConversions()
{
    for (int i = 0; i < device_cnt; ++i) {
        ds_devices[i].StartConversion();
    }
}

void DS18B20_scan::ReadTemperatures()
{
    for (int i = 0; i < device_cnt; ++i) {
        ds_devices[i].ReadTemperature();
    }
}

int DS18B20_scan::TemperatureValue (int device) {
    return ds_devices[device].TemperatureValue();
}

void DS18B20_scan::getTemperatureData (int16_t * data)
{
    int i;
    for ( i = 0; i < device_cnt; ++i) {
        data[i] = ds_devices[i].TemperatureValue();
    }
    for ( ; i < MAX_DS18B20; ++i) {
        data[i] = 0;
    }
}

bool DS18B20_scan::ScanDevices() {
    switch (scan_status) {
    case 1:
        StartConversions();
        conversion_time = millis() + 1000;
            scan_status = 2;
        break;

    case 2:
        if (millis() > conversion_time) {
            ReadTemperatures();
            scan_status = 1;
            return true;
        }
        break;

    default:
        break;
    }
    return false;
}
