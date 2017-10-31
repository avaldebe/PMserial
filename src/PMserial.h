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
// some ICs have only SWserial, eg tiny85
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

class SerialPM{
public:
  SerialPM(PMS sensor) : pms(sensor) {}
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
};

#endif //_SERIALPM_H
