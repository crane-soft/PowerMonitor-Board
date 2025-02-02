#include "DigitalOut.h"
#include "DigitalIn.h"
#include "BlinkLed.hpp"

using namespace mbed;

class PowerSW {
public:
    PowerSW();
    void WaitPowerOn();
    bool CheckPowerOFF();
    void PowerOFF();
    int PowerLed();

private:
    void StartPowerButton(int NewState, int NewTime);
    bool CheckPowerButton ();
    void SetPoweStatus1();
    int _PowerStatus;

    DigitalIn _PowerButton;
    DigitalOut _PowerHold; 
    BlinkLed _ExtPowerLed; 
    BlinkLed _BoardPwrLed;
    int _CheckState;
    int _Time_ms;
    unsigned long _DelTime;
    int _ExtLedOnTime;
};
