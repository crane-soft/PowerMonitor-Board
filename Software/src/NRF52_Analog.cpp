#include "NRF52_Analog.h"

int16_t *  csNRF52analog::GetAdcValues ()
{
  return (int16_t*)adcBuffer;
}

bool csNRF52analog::CheckAdc()
{
  // check if ADCs ready, if not oversapling to hight?
  return (NRF_SAADC->EVENTS_END != 0); 
}

void csNRF52analog::StartADC() 
{
    NRF_SAADC->EVENTS_END = 0;
    NRF_SAADC->TASKS_START = 1;
    NRF_SAADC->TASKS_SAMPLE = 1;
}

void csNRF52analog::ConfigChannel(int chan, int ANL_Input)
{
  NRF_SAADC->CH[chan].CONFIG = ( SAADC_CH_CONFIG_GAIN_Gain1_4   << SAADC_CH_CONFIG_GAIN_Pos   ) |
                               ( SAADC_CH_CONFIG_MODE_SE        << SAADC_CH_CONFIG_MODE_Pos   ) |
                               ( SAADC_CH_CONFIG_REFSEL_VDD1_4  << SAADC_CH_CONFIG_REFSEL_Pos ) |
                               ( SAADC_CH_CONFIG_RESN_Bypass    << SAADC_CH_CONFIG_RESN_Pos   ) |
                               ( SAADC_CH_CONFIG_RESP_Bypass    << SAADC_CH_CONFIG_RESP_Pos   ) |
                               ( SAADC_CH_CONFIG_TACQ_15us      << SAADC_CH_CONFIG_TACQ_Pos   ) |
                               ( SAADC_CH_CONFIG_BURST_Enabled  << SAADC_CH_CONFIG_BURST_Pos  );
  
  NRF_SAADC->CH[chan].PSELP = ANL_Input << SAADC_CH_PSELP_PSELP_Pos;
  NRF_SAADC->CH[chan].PSELN = SAADC_CH_PSELN_PSELN_NC << SAADC_CH_PSELN_PSELN_Pos;
}

void csNRF52analog::InitADC()
{
  nrf_saadc_disable();

  NRF_SAADC->RESOLUTION = NRF_SAADC_RESOLUTION_12BIT;

  ConfigChannel (3,SAADC_CH_PSELP_PSELP_AnalogInput3);  // A1 AIN3 (nich AIN2 )
  ConfigChannel (4,SAADC_CH_PSELP_PSELP_AnalogInput6);  // A2
  ConfigChannel (5,SAADC_CH_PSELP_PSELP_AnalogInput5);  // A3
  ConfigChannel (6,SAADC_CH_PSELP_PSELP_AnalogInput4);  // A6

  NRF_SAADC->OVERSAMPLE = 3;  // 2^3 = 8-fach ist das maximum für 1ms Sample rate
  NRF_SAADC->RESULT.MAXCNT = ADC_BUFFER_SIZE;
  NRF_SAADC->RESULT.PTR = ( uint32_t )&adcBuffer;

  NRF_SAADC->EVENTS_END = 0;

  nrf_saadc_enable();

  NRF_SAADC->TASKS_CALIBRATEOFFSET = 1;
  while ( NRF_SAADC->EVENTS_CALIBRATEDONE == 0 );
  NRF_SAADC->EVENTS_CALIBRATEDONE = 0;
  while ( NRF_SAADC->STATUS == ( SAADC_STATUS_STATUS_Busy << SAADC_STATUS_STATUS_Pos ) );
}
