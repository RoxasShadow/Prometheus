#include <Arduino.h>
#include <Wire.h>
#include "VCNL4000.h"

#define VCNL4000_ADDRESS 0x13

#define VCNL4000_COMMAND 0x80
#define VCNL4000_PRODUCTID 0x81
#define VCNL4000_IRLED 0x83
#define VCNL4000_AMBIENTPARAMETER 0x84
#define VCNL4000_AMBIENTDATA 0x85
#define VCNL4000_PROXIMITYDATA 0x87
#define VCNL4000_SIGNALFREQ 0x89
#define VCNL4000_PROXIMITYADJUST 0x8A

#define VCNL4000_3M125 0
#define VCNL4000_1M5625 1
#define VCNL4000_781K25 2
#define VCNL4000_390K625 3

#define VCNL4000_MEASUREAMBIENT 0x10
#define VCNL4000_MEASUREPROXIMITY 0x08
#define VCNL4000_AMBIENTREADY 0x40
#define VCNL4000_PROXIMITYREADY 0x20


VCNL4000::VCNL4000(){
  Wire.begin();
}

VCNL4000::~VCNL4000(){
}

uint16_t VCNL4000::proximity()
{
  VCNL4000::write8(VCNL4000_COMMAND, VCNL4000_MEASUREPROXIMITY);
  while (1) {
    uint8_t result = VCNL4000::read8(VCNL4000_COMMAND);
    if (result & VCNL4000_PROXIMITYREADY){
      return VCNL4000::read16(VCNL4000_PROXIMITYDATA);
    }
  }
}

uint16_t VCNL4000::ambient()
{
  VCNL4000::write8(VCNL4000_COMMAND, VCNL4000_MEASUREAMBIENT);
  while (1) {
    uint8_t result = VCNL4000::read8(VCNL4000_COMMAND);
    if (result & VCNL4000_AMBIENTREADY){
      return VCNL4000::read16(VCNL4000_AMBIENTDATA);
    }
  }
}

void VCNL4000::init()
{
  VCNL4000::setIRPower(0x14);
  VCNL4000::setProximityFreq(0x81);
  VCNL4000::setIRFreq(0x02);
  VCNL4000::setAmbientSamples(0x88);
}

void VCNL4000::setIRPower(uint8_t value)  //value * 10 = IR LED current in mA
{
  VCNL4000::write8(VCNL4000_IRLED, constrain(value, 0, 0x14));
}

void VCNL4000::setProximityFreq(uint8_t value)  //Mfgr recommends 0x81
{
  VCNL4000::write8(VCNL4000_PROXIMITYADJUST, value);
}

void VCNL4000::setIRFreq(uint8_t value)  //0:3M125 1:1M5625 2:781K25(default) 3:390K625, all in Hertz
{
  VCNL4000::write8(VCNL4000_SIGNALFREQ, value);
}

void VCNL4000::setAmbientSamples(uint8_t value)  //Ambient light sensor settings.  last 3 bits = x.  2^x samples per 100 ms period
{
  VCNL4000::write8(VCNL4000_AMBIENTPARAMETER, value);
}

uint8_t VCNL4000::read8(uint8_t address)
{
  Wire.beginTransmission(VCNL4000_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  delayMicroseconds(170);
  Wire.requestFrom(VCNL4000_ADDRESS, 1);
  while(!Wire.available());
  return Wire.read();
}

uint16_t VCNL4000::read16(uint8_t address)
{
  uint16_t data;

  Wire.beginTransmission(VCNL4000_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(VCNL4000_ADDRESS, 2);
  while(!Wire.available());
  data = Wire.read();
  data <<= 8;
  while(!Wire.available());
  data |= Wire.read();
  
  return data;
}

void VCNL4000::write8(uint8_t address, uint8_t data)
{
  Wire.beginTransmission(VCNL4000_ADDRESS);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}

boolean VCNL4000::available()
{
  return Wire.available();
}
