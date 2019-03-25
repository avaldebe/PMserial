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
#ifdef HAS_HW_SERIAL
#include <HardwareSerial.h>
#endif

#if defined(__AVR__) || defined(ESP8266)
#define HAS_SW_SERIAL
#include <SoftwareSerial.h>
#endif

#if defined(HAS_HWSERIAL1) || defined(BOARD_HAVE_USART1)
#define HAS_HW_SERIAL1
#endif

// leonardo & maple_mini: Serial1 is HWserial
#if defined(__AVR_ATmega32U4__) || defined(SERIAL_USB)
#define HAS_USB_SERIAL
#endif

enum PMS {
  PLANTOWER_24B, // 24bit long message, no count info (LBC)
  PLANTOWER_32B, // 32bit long message, w/count info (LBC)
  PMS1003=PLANTOWER_32B, // G1
  PMS3003=PLANTOWER_24B, // G3
  PMS5003=PLANTOWER_32B, // G5
  PMS7003=PLANTOWER_32B, // G7
  PMSA003=PLANTOWER_32B  // G10
};

class SerialPM{
public:
  uint16_t
    pm[3],  // particulate matter [ug/m3]
    nc[6];  // number concentration [#/100cc]
#ifdef HAS_HW_SERIAL
  SerialPM(PMS sensor, HardwareSerial &serial) : pms(sensor) {
    uart=&serial;
    hwSerial=true;
  }
#endif
#ifdef HAS_SW_SERIAL
  SerialPM(PMS sensor, uint8_t rx, uint8_t tx) : pms(sensor) {
    SoftwareSerial serial(rx,tx);
    uart=&serial;
    hwSerial=false;
  }
#endif
  void init();
  enum STATUS {
    OK,
    ERROR_TIMEOUT,
    ERROR_MSG_HEADER,
    ERROR_MSG_BODY,
    ERROR_MSG_START,
    ERROR_MSG_LENGHT,
    ERROR_MSG_CKSUM
  };
  STATUS read(bool tsi_mode=false, bool truncated_num=false);

protected:
  Stream *uart; // hardware/software serial
  PMS pms;
  static const uint8_t BUFFER_LEN = 32;
  uint8_t buffer[BUFFER_LEN];
  bool has_num, hwSerial;

  // utility functions
  STATUS trigRead();
  bool checkBuffer(size_t bufferLen);
  void decodeBuffer(bool tsi_mode=false, bool truncated_num=false);
};

#endif //_SERIALPM_H
