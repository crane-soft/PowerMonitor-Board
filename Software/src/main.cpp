#include <Arduino.h>
#include "PowerSW.h"
#include "BleHandler.h"
#include "BlinkLed.hpp"

// Built by Khoi Hoang https://github.com/khoih-prog/NRF52_MBED_TimerInterrupt

#include "NRF52_MBED_TimerInterrupt.h"
#include "NRF52_Analog.h"
#define POWER_SW

NRF52_MBED_Timer ITimer1(NRF_TIMER_3);
csNRF52analog NRF52analog;
PowerSW BoardPW;
csBleHandler BleHandler;

BlinkLed BlueLed(PinName::LED_BLUE,0,1);
BlinkLed ErrorLed(PinName::LED_RED,0,1);
bool triggerBle;

void TimerHandler() 
{
  static int timerCnt;
  BleHandler.UpdateData(NRF52analog.GetAdcValues());

  ErrorLed = (NRF52analog.CheckAdc() == false);
  NRF52analog.StartADC();

  if (++ timerCnt == 200) {
    timerCnt = 0;
    triggerBle = true;
  }
}

void setup() 
{
  ErrorLed = 0;
#ifdef POWER_SW  
  BoardPW.WaitPowerOn();
#endif  
  //Serial.begin (9600);
  //Serial.println ("E-Bike Power Monitor");
  BleHandler.setup();
  NRF52analog.InitADC();
  ITimer1.attachInterruptInterval(1000, TimerHandler);
}

void loop() 
{
  __WFI();  
#ifdef POWER_SW  
  if (BoardPW.CheckPowerOFF() == true) {
      BoardPW.PowerOFF();
  }
#endif

  if (triggerBle) {
    triggerBle = false;
    //BlueLed.Toggle();
    BleHandler.Execute();
  } else {
    BleHandler.ScanTemperDevices();
  }
}
