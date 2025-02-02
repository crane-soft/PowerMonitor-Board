#include <Arduino.h>
#include "csAverage.hpp"

class csBattery {
public:
    void update(int16_t * adcValues);
    int voltage();
    int level();
    void getBleData (int16_t * bleData);

private:
    uint16_t m_adc_value;
    int m_voltage;
    csAverageBuff<uint16_t,1000> m_adcAvrBuff;
};
