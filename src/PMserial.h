/* SerialPM
 Arduino library for PM sensors with serial interface
  PMS1003 aka G1
  PMS3003 aka G2
  PMS5003 aka G5
  PMS7003 aka G7
  PMSA003 aka G10
*/
#ifndef _SERIALPM_H
#define _SERIALPM_H

#include <Arduino.h>

#define HAS_HW_SERIAL

#if defined(__AVR__) || defined(ESP8266)
#define HAS_SW_SERIAL
#include <SoftwareSerial.h>
#endif

// leonardo & maple_mini: Serial1 is HWserial
#if defined(__AVR_ATmega32U4__) || defined(SERIAL_USB)
#define HAS_USB_SERIAL
#endif

enum PMS {
  PLANTOWER_24B, // 24bit long message, no count info (psd)
  PLANTOWER_32B, // 32bit long message, w/count info (psd)
  PMS1003=PLANTOWER_32B, G1=PMS1003,
  PMS3003=PLANTOWER_24B, G3=PMS3003,
  PMS5003=PLANTOWER_32B, G5=PMS5003,
  PMS7003=PLANTOWER_32B, G7=PMS7003,
  PMSA003=PLANTOWER_32B, G10=PMSA003
};

class SerialPM{
public:
  uint16_t pm_tsi[3], pm_atm[3], psd[6];
  boolean has_psd;
  SerialPM(PMS sensor) : pms(sensor) {
    switch (pms) {
    case PLANTOWER_24B:
      bufferLen=24;
      has_psd=false;
    default:
      bufferLen=32;
      has_psd=true;
    }
  }
#ifdef HAS_HW_SERIAL
  void begin(HardwareSerial &serial);
#endif
#ifdef HAS_SW_SERIAL
  void begin(SoftwareSerial &serial);
#endif
  void init();
  void read();

protected:
  Stream *uart; // hardware/software serial
  PMS pms;
  uint8_t bufferLen, buffer[32];

  // utility functions
  void trigRead(const uint8_t *message, uint8_t lenght);
  boolean checkBuffer();
};

#endif //_SERIALPM_H
