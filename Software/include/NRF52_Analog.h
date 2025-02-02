#include "mbed.h"

#define ADC_BUFFER_SIZE     4

class csNRF52analog {
public:
    void InitADC();
    bool CheckAdc();
    void StartADC();
    int16_t * GetAdcValues ();
private:
    void ConfigChannel(int chan, int ANL_Input);
    volatile nrf_saadc_value_t adcBuffer[ADC_BUFFER_SIZE];
};
