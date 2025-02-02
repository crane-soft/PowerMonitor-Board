#include <Arduino.h>

class ClockTimer {
public:
    ClockTimer() {
        milliseconds = 0;
        seconds = 0;
        minutes = 0;
        hours = 0;
    }

    void Clock_ms () {
        if (++milliseconds == 1000) {
            milliseconds = 0;
            if (++seconds == 60) {
                seconds = 0;
                if (++minutes == 60) {
                    minutes = 0;
                    ++hours;
                }
            }
        }
    }

    void getBleData (uint8_t * bleData)
    {
        bleData[0] = hours;
        bleData[1] = minutes;
        bleData[2] = seconds;
    }

private:
    int milliseconds;
    int seconds;
    int minutes;
    int hours;
};