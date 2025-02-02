#include "csBattery.h"
#include "BlinkLed.hpp"
#include "ADC_Chans.h"

void csBattery::update(int16_t * adcValues)
{
    m_adcAvrBuff.AddValue(adcValues[ADC_Voltage_Chan]);
    m_adc_value = m_adcAvrBuff.GetAvrValue();
    m_voltage = (int) map(m_adc_value, 0, 3623, 0, 420);
 
}

int csBattery::voltage()
{
    return m_voltage;
}

int csBattery::level()
{
    if (m_voltage <= 280)
        return 0;
    if (m_voltage >= 420)
        return 100;
    return map(m_voltage, 280, 420, 0, 100);
}

void csBattery::getBleData (int16_t * bleData)
{
    bleData[0] = level();
    bleData[1] = voltage();
}
