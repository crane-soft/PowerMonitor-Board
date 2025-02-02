#include <mbed.h>
#include "PowerSW.h"

#define POWER_BUTTON  PinName::P1_11  //D2
#define POWER_HOLD    PinName::P0_23  // D7

#define EXT_POWER_LED PinName::P0_21  // D8
#define BOARD_PWR_LED PinName::P1_9

#define ON_DELAY 2000
#define RELEASE_DELAY 1000
#define OFF_DELAY 2000

PowerSW::PowerSW (): 
  _PowerButton (POWER_BUTTON),
  _PowerHold (POWER_HOLD), 
  _ExtPowerLed (EXT_POWER_LED),
  _BoardPwrLed (BOARD_PWR_LED)
{
    _PowerStatus = 0;
}

void PowerSW::WaitPowerOn()
{
    StartPowerButton (LOW, ON_DELAY);
    while (CheckPowerButton () == false) {
        continue;
    }

    _PowerHold = 1;
    _ExtPowerLed = 1;
    _BoardPwrLed = 1;
    // Power Button needs to be released a while
    // before again checked for power OFF
    StartPowerButton (HIGH, RELEASE_DELAY);
    _PowerStatus = 0;
}

bool PowerSW::CheckPowerOFF()
{
  if (_PowerStatus == 0) {
    if (CheckPowerButton () == true) {
      StartPowerButton (LOW, OFF_DELAY);
      SetPoweStatus1();
    }
  } else {
    _BoardPwrLed.blink();
    if (_PowerStatus == 1) {
      _ExtPowerLed =  _BoardPwrLed;
      if (millis() > _ExtLedOnTime) {
        _BoardPwrLed.blinkTime (5000,50);
        _ExtPowerLed = 0;
        _PowerStatus = 2;
      }


    } else {
        if (_PowerButton == LOW) {
          SetPoweStatus1();
        }
    }

    if (CheckPowerButton () == true) {
      return true;
    }
  }
  return false;
}

void PowerSW::SetPoweStatus1()
{
  _PowerStatus = 1;
  _BoardPwrLed.blinkTime (1000,500);
  _ExtLedOnTime = millis() + 5000;
}

void  PowerSW::PowerOFF()
{
  _PowerHold = 0;
  _BoardPwrLed.blinkTime (200,100);
  while (_PowerButton == 0) 
  {
    _BoardPwrLed.blink();
    _ExtPowerLed = _BoardPwrLed;
  }
  _ExtPowerLed = 0;
  _BoardPwrLed = 1;
  while (1);
}

  int PowerSW::PowerLed()
  {
    return _ExtPowerLed.read();
  }

void PowerSW::StartPowerButton(int NewState, int NewTime)
{
    _CheckState = NewState;
    _Time_ms = NewTime;
    _DelTime = millis() + _Time_ms;
}

bool PowerSW::CheckPowerButton ()
{
  unsigned long millisNow = millis();
  if (_PowerButton != _CheckState)
  {
    _DelTime = millisNow + _Time_ms;
  } else {
    if (_DelTime > 0) {
      if (millisNow > _DelTime) {
        return true;
      }
    }
  }
  return false;
}
