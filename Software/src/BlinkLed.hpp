#ifndef BLINKLED_H
#define BLINKLED_H
#include <Arduino.h>
#include "DigitalOut.h"

using namespace mbed;
class BlinkLed : public DigitalOut {
public:
    BlinkLed (PinName pin) :DigitalOut(pin) {}
    BlinkLed (PinName pin, int value) :DigitalOut(pin,value) {}
    BlinkLed (PinName pin, int value, int polarity) :DigitalOut(pin) { 
            _polarity = polarity;
            write_pin (value);
          }

    void blinkTime(unsigned cycleTime,unsigned onTime) {
        _onTime = onTime;
        _offTime = cycleTime - _onTime;
        _blinkTime = 0;
    }

    void blink() {
        if (millis() > _blinkTime) {
            if (_value == 0) {
                write_pin(1);
                _blinkTime =  millis() + _onTime;
            } else {
                write_pin(0);
                _blinkTime =  millis() + _offTime;
            }
        }
    }

    void  operator = (int value) {
        write_pin(value);
    }
    
    void Toggle () 
    {
        write_pin (!_value);
    }

private:
    void write_pin (int value) {
        _value = value;
        write (value ^_polarity);
    }
    int _polarity;
    int _value;
    unsigned _onTime;
    unsigned _offTime;
    unsigned _blinkTime;
};


#endif // BLINKLED_H
