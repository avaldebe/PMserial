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
SerialPM pms(PMS5003);

#ifdef HAS_SW_SERIAL
SoftwareSerial SWSerial(10,11);
#endif

void setup() {
  Serial.begin(9600);
  Serial.println("Booted");

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

void loop() {
  // read the PM sensor
  pms.read();
  Serial.print("PM1 ")  ;Serial.print(pms.pm_atm[0]);Serial.print(", ");
  Serial.print("PM2.5 ");Serial.print(pms.pm_atm[1]);Serial.print(", ");
  Serial.print("PM10 ") ;Serial.print(pms.pm_atm[2]);Serial.println(" [ug/m3]");

  // wait for 10 seconds
  delay(10000);
}
