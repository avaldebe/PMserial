/* SerialPM
 Arduino library for PM sensors with serial interface
  PMS1003 aka G1
  PMS3003 aka G2
  PMS5003 aka G5
  PMS7003 aka G7
  PMSA003 aka G10
*/
#include <Arduino.h>

#include <PMserial.h>
SerialPM pms(PMS3003);

#ifdef HAS_SW_SERIAL
SoftwareSerial SWSerial(0,1);
#endif

void setup()
{
#ifdef HAS_USB_SERIAL         // leonardo & maple_mini: Serial1 is HWserial
  Serial.println("PMS sensor on HardwareSerial1");
  pms.begin(Serial1);
#elif defined(HAS_SW_SERIAL)  // test SWserial
  Serial.println("PMS sensor on SoftwareSerial");
  pms.begin(SWSerial);
#else
  Serial.println("PMS sensor on HardwareSerial");
  pms.begin(Serial);
#endif
  pms.init();
}

void loop()
{
  // read the PM sensor
  pms.read();

  // wait for 10 seconds
  delay(10000);
}
