#include "PowerMeter.h"
#include "ADC_Chans.h"
// 
csPowerMeter::csPowerMeter(int adc_chan)
{
    m_adc_chan = adc_chan;
    m_energy_accu = 0;
    m_currentBuff.ClearLevel();
}

void csPowerMeter::update(int16_t * adcValues, int voltage)
{
    m_voltage = voltage;
    m_currentBuff.AddValue(adcValues[m_adc_chan]);
    m_adc_value = m_currentBuff.GetAvrValue();
    m_current = Adc2Current (m_adc_value);
    m_energy_accu += m_current;
}

int csPowerMeter::Adc2Current (uint16_t adc_value)
{
    // return value [0.01 A]   map values see excel sheet
    int current_val = map(adc_value, 465, 4189, 0, 500);

    //if (current_val < 0) {
    //    current_val = 0;
    //}
    return current_val; 
}

int csPowerMeter::current()
{
    return m_current;
}

int csPowerMeter::power() 
{
    // power = 1/10 Watt
    return (m_current * m_voltage) / 100;
}
int csPowerMeter::energy()
{
    // enrgy = 1/10 Wh           1/10W  1000ms 60s 60m
    //int64_t i64_energy = m_energy_accu / 1000 /60 / 60;
    //return (int)(i64_energy * m_voltage);
    int64_t i64_energy = m_energy_accu * m_voltage / 100 / 1000 /60 / 60;
    if (i64_energy > 65000)
        i64_energy = 65000;
    return (int) i64_energy;
}

void csPowerMeter::getBleData (int16_t * bleData)
{
    bleData[0] = current();
    bleData[1] = power();
    bleData[2] = energy();
}

