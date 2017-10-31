/* SerialPM
 Arduino library for PM sensors with serial interface
  PMS1003 aka G1
  PMS3003 aka G2
  PMS5003 aka G5
  PMS7003 aka G7
  PMSA003 aka G10
*/
#include <PMserial.h>

const uint8_t
  msgLen = 7,
  cfg[msgLen] = {0x42,0x4D,0xE1,0x01,0x00,0x01,0x72},
  trg[msgLen] = {0x42,0x4D,0xE2,0x00,0x00,0x01,0x72}; // pasive mode read

#ifdef HAS_HW_SERIAL
void SerialPM::begin(HardwareSerial &serial){
  serial.begin(9600,SERIAL_8N1);
  uart = &serial;
  init();
}
#endif

#ifdef HAS_SW_SERIAL
void SerialPM::begin(SoftwareSerial &serial){
  serial.begin(9600);
  uart = &serial;
  init();
}
#endif

void SerialPM::init(){
  uart->write(cfg,msgLen);
}

void SerialPM::read(){
  uart->write(trg,msgLen);
}
