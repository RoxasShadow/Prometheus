#ifndef H_VCNL4000
#define H_VCNL4000

#include <Arduino.h>

class VCNL4000
{
  public:

    VCNL4000();
    ~VCNL4000();

    uint16_t proximity();
    uint16_t ambient();

    void init();
    void setIRPower(uint8_t);
    void setProximityFreq(uint8_t);
    void setIRFreq(uint8_t);
    void setAmbientSamples(uint8_t);

    uint8_t read8(uint8_t address);
    uint16_t read16(uint8_t address);
    void write8(uint8_t address, uint8_t data);
    boolean available();
};

#endif /* H_VCNL4000 */
