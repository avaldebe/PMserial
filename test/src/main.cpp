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

#ifdef ESP32
#define HAS_HW_SERIAL2
#elif defined(HAS_SW_SERIAL)
SoftwareSerial SWSerial(10,11);
#endif

void setup() {
  Serial.begin(9600);
  Serial.println(F("Booted"));

#if   defined(HAS_HW_SERIAL2) // esp32: Serial2 is HWserial
  Serial.println(F("PMS sensor on HardwareSerial2"));
  pms.begin(Serial2);
#elif defined(HAS_HW_SERIAL1) // leonardo & maple_mini: Serial1 is HWserial
  Serial.println(F("PMS sensor on HardwareSerial1"));
  pms.begin(Serial1);
#elif defined(HAS_SW_SERIAL)  // test SWserial
  Serial.println(F("PMS sensor on SoftwareSerial"));
  pms.begin(SWSerial);
#else
  Serial.println(F("PMS sensor on HardwareSerial"));
  pms.begin(Serial);
#endif
  pms.init();
}

void loop() {
  // read the PM sensor
  pms.read();
  Serial.print(F("PM1 "))  ;Serial.print(pms.pm[0]);Serial.print(F(", "));
  Serial.print(F("PM2.5 "));Serial.print(pms.pm[1]);Serial.print(F(", "));
  Serial.print(F("PM10 ")) ;Serial.print(pms.pm[2]);Serial.println(F(" [ug/m3]"));

  // wait for 10 seconds
  delay(10000);
}
