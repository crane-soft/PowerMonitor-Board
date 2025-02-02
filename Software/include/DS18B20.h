#ifndef DS18B20_h
#define DS18B20_h
#include <Arduino.h>
#include "OneWire.h"

class DS18B20 {
public:
    DS18B20();
    bool Search_Device(OneWire * ds);
    void StartConversion();
    int ReadTemperature();
    int TemperatureValue() {
        return m_TemperatureValue;
    }
private:
    OneWire  *m_OneWire;
    enum DS_chip{
        enDSunknown = 0,
        enDS18S20,
        enDS18B20,
        enDS1822
    };

    uint8_t rom [8];
    uint8_t type_s;
    DS_chip chip;
    int m_TemperatureValue;
};

#endif // DS18B20_h
