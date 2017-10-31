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
SerialPM pms(PMS1003);

void setup()
{
// leonardo & maple_mini: Serial1 is HWserial
#ifdef HAS_USB_SERIAL
  pms.begin(Serial1);
#else
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
