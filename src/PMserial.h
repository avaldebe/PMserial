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

#if (defined(__AVR__) || defined(ESP8266)) && !defined(NO_SW_SERIAL_REQUIRED)
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

enum PMS
{
  PLANTOWER_AUTO,             // self discovery
  PLANTOWER_24B,              // 24 byte long message, no count info (LBC)
  PLANTOWER_32B,              // 32 byte long message, w/count info (LBC)
  PLANTOWER_32B_S,            // 32 byte long message, w/count info and HCHO (LBC)
  PLANTOWER_32B_T,            // 32 byte long message, w/partial count info, temp and rhum (LBC)
  PLANTOWER_40B,              // 40 byte long message, w/count info, temp, rhum and HCHO (LBC)
  PMSx003 = PLANTOWER_AUTO,   // self discovery
  PMS1003 = PLANTOWER_32B,    // G1
  PMS3003 = PLANTOWER_24B,    // G3
  PMS5003 = PLANTOWER_32B,    // G5
  PMS5003S = PLANTOWER_32B_S, // G5S
  PMS5003T = PLANTOWER_32B_T, // G5T
  PMS5003ST = PLANTOWER_40B,  // G5ST
  PMS7003 = PLANTOWER_32B,    // G7
  PMSA003 = PLANTOWER_32B     // G10
};

class SerialPM
{
public:
  union
  {
    uint16_t data[9]; // all PM/NC data
    struct
    {
      uint16_t pm[3]; // particulate matter [ug/m3]
      uint16_t nc[6]; // number concentration [#/100cc]
    };
    struct
    {
      // pmX [ug/m3]: PM1.0, PM2.5 & PM10
      uint16_t pm01, pm25, pm10;
      // nXpY [#/100cc]: number concentrations under X.Y um
      uint16_t n0p3, n0p5, n1p0, n2p5, n5p0, n10p0;
    };
  };
  union
  {
    float extra[3]; // T/RH/HCHO
    struct
    {
      // temperature [°C], relative humidity [%], formaldehyde concentration [mg/m3]
      float temp, rhum, hcho;
    };
  };

  // manual mode - you need configure SerialPort manual and link to this object (function "setSerialPort")
  SerialPM(PMS sensor) : pms(sensor)
  {
    uart = nullptr;
    hwSerial = serModeManual;
  }

#ifdef HAS_HW_SERIAL
  SerialPM(PMS sensor, HardwareSerial &serial) : pms(sensor)
  {
    uart = &serial;
    hwSerial = serModeHardware;
  }
#endif

#ifdef HAS_SW_SERIAL
  SerialPM(PMS sensor, uint8_t rx, uint8_t tx) : pms(sensor)
  {
    static SoftwareSerial serial(rx, tx);
    uart = &serial;
    hwSerial = serModeSoftware;
  }

  SerialPM(PMS sensor, Stream &serial) : pms(sensor)
  {
    uart = &serial;
    //TODO: WIP!!!
    hwSerial = serModeManual;
  }
#elif defined(ESP32)
  SerialPM(PMS sensor, uint8_t rx, uint8_t tx) : pms(sensor), rx(rx), tx(tx)
  {
    uart = &Serial1;
    hwSerial = serModeHardware;
  }
#endif

  void init();

#define PMS_ERROR_TIMEOUT "Sensor read timeout"
#define PMS_ERROR_PMS_TYPE "Wrong PMSx003 sensor type"
#define PMS_ERROR_MSG_UNKNOWN "Unknown message protocol"
#define PMS_ERROR_MSG_HEADER "Incomplete message header"
#define PMS_ERROR_MSG_BODY "Incomplete message body"
#define PMS_ERROR_MSG_START "Wrong message start"
#define PMS_ERROR_MSG_LENGTH "Message too long"
#define PMS_ERROR_MSG_CKSUM "Wrong message checksum"

  enum STATUS
  {
    OK,
    ERROR_TIMEOUT,
    ERROR_PMS_TYPE,
    ERROR_MSG_UNKNOWN,
    ERROR_MSG_HEADER,
    ERROR_MSG_BODY,
    ERROR_MSG_START,
    ERROR_MSG_LENGTH,
    ERROR_MSG_CKSUM
  };

  STATUS status;
  STATUS read(bool tsi_mode = false, bool truncated_num = false);
  operator bool() { return status == OK; }
  void sleep();
  void wake();
  inline bool has_particulate_matter() { return status == OK; }
  inline bool has_number_concentration() { return (status == OK) && (pms != PMS3003); }
  inline bool has_temperature_humidity() { return (status == OK) && ((pms == PMS5003T) || (pms == PMS5003ST)); }
  inline bool has_formaldehyde() { return (status == OK) && ((pms == PMS5003S) || (pms == PMS5003ST)); }
  inline Stream * getSerialPort() { return uart; }
  inline void setSerialPort(Stream * serial) { uart = serial; hwSerial = serModeManual; }
    
  // adding offsets works well in normal range
  // might introduce under- or overflow at the ends of the sensor range
  inline void  set_rhum_offset(float offset) { rhum_offset = offset; };
  inline void  set_temp_offset(float offset) { temp_offset = offset; };
  inline float get_rhum_offset() { return rhum_offset; };
  inline float get_temp_offset() { return temp_offset; };
#ifdef PMS_DEBUG
#ifdef HAS_HW_SERIAL
  inline void print_buffer(Stream &term, const char *fmt)
  {
    char tmp[8];
    for (uint8_t n = 0; n < BUFFER_LEN; n += 2)
    {
      sprintf(tmp, fmt, buff2word(n));
      term.print(tmp);
    }
  }
#endif
  // debug timing
  inline uint16_t waited_ms() { return wait_ms; }
  // debug readBytes
  inline uint16_t bytes_read() { return nbytes; }
#endif

protected:
  Stream *uart;  // hardware/software serial
  PMS pms;       // sensor type/message protocol
  enum {serModeHardware, serModeSoftware, serModeManual} hwSerial; // Is uart hardware serial? (or software serial)
#ifdef ESP32
  uint8_t rx, tx; // Serial1 pins on ESP32
#endif

  // Correct Temperature & Humidity
  float temp_offset = 0.0;
  float rhum_offset = 0.0;

  // utility functions
  STATUS trigRead();
  bool checkBuffer(size_t bufferLen);
  void decodeBuffer(bool tsi_mode, bool truncated_num);

  // message timing
  static const uint16_t max_wait_ms = 1000;
  uint16_t wait_ms; // time spent waiting for new sample

  // message buffer
  static const uint8_t BUFFER_LEN = 40;
  uint8_t buffer[BUFFER_LEN], nbytes;
  inline uint16_t buff2word(uint8_t n) { return (buffer[n] << 8) | buffer[n + 1]; }
};

#endif //_SERIALPM_H
