/* SerialPM
 Arduino library for PM sensors with serial interface
  PMS1003 aka G1
  PMS3003 aka G2
  PMS5003 aka G5
  PMS7003 aka G7
  PMSA003 aka G10
*/
#include "Arduino.h"

//** PMserial.h

enum PMS {
  PMS1003, G1=PMS1003,
  PMS3003, G3=PMS3003,
  PMS5003, G5=PMS5003,
  PMS7003, G7=PMS7003,
  PMSA003, G10=PMSA003
};


class SerialPM{
public:
  SerialPM(Stream *serial, PMS sensor) :
    uart(serial), pms(sensor) {}
  SerialPM(Stream &serial, PMS sensor) :
    uart(&serial), pms(sensor)  {}

  void init();
  void read();

protected:
  Stream *uart; // hardware/software serial
  PMS pms;
};

//*** PMserial.cpp

const uint8_t msgLen=7;
const uint8_t
  cfg[msgLen] = {0x42,0x4D,0xE1,0x01,0x00,0x01,0x72},
  trg[msgLen] = {0x42,0x4D,0xE2,0x00,0x00,0x01,0x72}; // pasive mode read

void SerialPM::init(){
  uart->write(cfg,msgLen);
}

void SerialPM::read(){
  uart->write(trg,msgLen);
}


//*** example

SerialPM pms(Serial,PMS1003);

void setup()
{  
  Serial.begin(9600,SERIAL_8N1);
  pms.init();
}

void loop()
{
  // read the PM sensor 
  pms.read();

  // wait for 10 seconds
  delay(10000);
}
