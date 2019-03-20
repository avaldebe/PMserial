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
#if   defined(ESP32)          // esp32
  #define MSG "PMS sensor on HardwareSerial2"
  SerialPM pms(PMS5003, Serial2);
#elif defined(HAS_HW_SERIAL1) // leonardo & maple_mini
  #define MSG "PMS sensor on HardwareSerial"
  SerialPM pms(PMS5003, Serial1);
#elif defined(HAS_SW_SERIAL)
  #define MSG "PMS sensor on SoftwareSerial"
  SoftwareSerial SWSerial(10,11);
  SerialPM pms(PMS5003, SWSerial);
#else
  #define MSG "PMS sensor on SoftwareSerial"
  SerialPM pms(PMS5003, Serial);
#endif

void setup() {
  Serial.begin(9600);
  Serial.println(F("Booted"));

  Serial.println(F(MSG));
  pms.begin();
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
