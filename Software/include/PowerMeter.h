#include <Arduino.h>
#include "csAverage.hpp"

class csPowerMeter {
public:
    csPowerMeter(int adc_chan);
    void update(int16_t * adcValues, int voltage);
    void getBleData (int16_t * bleData);
    int current();
    int power();
    int energy();
private:
    int Adc2Current (uint16_t adc_value);
    uint16_t m_adc_value;    
    int m_adc_chan;
    int m_current;
    int m_voltage;
    csAverageBuff<uint16_t,1000> m_currentBuff;
    int64_t m_energy_accu;
};
