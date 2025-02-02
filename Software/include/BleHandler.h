#include <ArduinoBLE.h>
#include "csBattery.h"
#include "PowerMeter.h"
#include "BlinkLed.hpp"
#include "ClockTimer.hpp"
#include "DS18B20_scan.h"

class csBleHandler
{
public:
    csBleHandler();
    void setup();
    void Execute();
    void ScanTemperDevices();
    void UpdateData(int16_t * adcValues);

private:
    ClockTimer clock;
    
    typedef __attribute__ ((__packed__)) struct {
        uint8_t flags;
        uint8_t clkData[3];
        int16_t tempData[MAX_DS18B20];
        int16_t data[11];
    } eBikeData_t;

    void SendData();
    BlinkLed BoardLed;
    BLEDevice HandyDevice;
    bool connected;
    eBikeData_t eBikeData;
    BLEService eBikeService;
    BLECharacteristic eBikeCharacteristic;

    csBattery Battery;
    csPowerMeter Motor;
    csPowerMeter Solar1;
    csPowerMeter Solar2;

    DS18B20_scan TemperScan;
};


