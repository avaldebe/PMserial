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
  PMS1003, G1=PMS1003,
  PMS3003, G3=PMS3003,
  PMS5003, G5=PMS5003,
  PMS7003, G7=PMS7003,
  PMSA003, G10=PMSA003
};
const uint8_t bufferLenMin=24, bufferLenMax=32;

class SerialPM{
public:
  uint16_t pm_tsi[3]={0xFFFF}, pm_atm[3]={0xFFFF}, psd[6]={0xFFFF};
  boolean has_psd=false;
  SerialPM(PMS sensor) : pms(sensor) {
    switch (pms) {
    case PMS3003:
      bufferLen=bufferLenMin;
      has_psd=false;
      break;
    default:
      bufferLen=bufferLenMax;
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
  uint8_t bufferLen, buffer[bufferLenMax];
  void trigRead(const uint8_t *message, uint8_t lenght);
  boolean checkBuffer();
};

#endif //_SERIALPM_H
