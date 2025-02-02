#include <Arduino.h>
#include <BleHandler.h>
#include "ADC_Chans.h"

#undef LED_BUILTIN

csBleHandler::csBleHandler():  
    BoardLed (PinName::LED_BUILTIN,0),
    eBikeService ("180F"),
    eBikeCharacteristic ("2A19",  BLERead | BLENotify,sizeof(eBikeData)),
    Motor(ADC_Motor_Chan),
    Solar1(ADC_Solar1_Chan),
    Solar2(ADC_Solar2_Chan)
{
}

void csBleHandler::setup()
{
    TemperScan.EnumDevices();

    if (BLE.begin()) {
        BLE.setLocalName("eBikeMonitor");
        BLE.setAdvertisedService(eBikeService); 
        eBikeService.addCharacteristic(eBikeCharacteristic); 
        BLE.addService(eBikeService);      
        BLE.advertise();
        BoardLed.blinkTime(1500,10);
        connected = false;
    }
}

void csBleHandler::ScanTemperDevices()
{
    TemperScan.ScanDevices();
}

void  csBleHandler::Execute()
{
    if (connected == false) {
        BoardLed.blink();
        HandyDevice = BLE.central();
        if (! HandyDevice) 
            return;
        BoardLed = 1;
        connected = true;
    } 

    if (HandyDevice.connected()) {            
        SendData();
    } else {
        connected = false;
        BoardLed.blinkTime(1500,10);
    }
}

void csBleHandler::UpdateData(int16_t * adcValues)
{
    clock.Clock_ms();
    Battery.update(adcValues);
    Motor.update (adcValues,Battery.voltage());
    Solar1.update (adcValues,Battery.voltage());
    Solar2.update (adcValues,Battery.voltage());
}

void csBleHandler::SendData()
{
    eBikeData.flags = 0;
    clock.getBleData (eBikeData.clkData);
    TemperScan.getTemperatureData (eBikeData.tempData);

    Battery.getBleData (&eBikeData.data[0]);
    Motor.getBleData (&eBikeData.data[2]);
    Solar1.getBleData (&eBikeData.data[5]);
    Solar2.getBleData (&eBikeData.data[8]);

    eBikeCharacteristic.writeValue((uint8_t*) &eBikeData, sizeof(eBikeData));     
}
