#include "DS18B20.h"

DS18B20::DS18B20()
{
    chip = enDSunknown;
}

bool DS18B20::Search_Device(OneWire *ds) 
{
  m_OneWire = ds;
  if (!m_OneWire->search(rom))
    return false;

 	//Serial.print("ROM =");
  for( int i = 0; i < 8; i++) {
    //Serial.write(' ');
    //Serial.print(rom[i], HEX);
  }

  if (OneWire::crc8(rom, 7) != rom[7]) {
    //Serial.println("CRC is not valid!");    
    return false;
  }

  // the first ROM byte indicates which chip
  switch (rom[0]) {
    case 0x10:
        //Serial.println("  Chip = DS18S20");  // or old DS1820
        chip =enDS18S20;
        type_s = 1;
        break;
    case 0x28:
        //Serial.println("  Chip = DS18B20");
        chip =enDS18B20;
        type_s = 0;
        break;
    case 0x22:
        //Serial.println("  Chip = DS1822");
        chip =enDS1822;
        type_s = 0;
        break;
    default:
        //Serial.println("Device is not a DS18x20 family device.");
        chip = enDSunknown;
        return false;
  } 
  return true;
}

void  DS18B20::StartConversion()
{
    m_OneWire->reset();
    m_OneWire->select(rom);
    m_OneWire->write(0x44);
}

int DS18B20::ReadTemperature ()
{
  if (m_OneWire->reset() == false) {
    return 0;
  }
  m_OneWire->select(rom);    
  m_OneWire->write(0xBE);         // Read Scratchpad

  uint8_t data[9];
  for ( int i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = m_OneWire->read();
  }

  if (OneWire::crc8(data, 8) != data[8]) {
    return 0;
  }


  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
   
    switch (cfg) {
      case 0x00:
        raw = raw & ~7;  // 9 bit resolution, 93.75 ms
        break;
      case 0x20:
        raw = raw & ~3; // 10 bit res, 187.5 ms
        break;
      case 0x40:
        raw = raw & ~1; // 11 bit res, 375 ms
        break;
      // default is 12 bit resolution, 750 ms conversion time
    }
  }

  m_TemperatureValue = (raw *10) / 16;
  return m_TemperatureValue;
}
