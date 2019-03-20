// HardwareSerial1.ino: Read PMS5003 sensor on Serial1

#include <PMserial.h>  // Arduino library for PM sensors with serial interface
SerialPM pms(PMS5003); // aka G5

void setup() {
  Serial.begin(9600);
  Serial.println(F("Booted"));

#ifdef ESP32
  Serial.println(F("PMS sensor on HardwareSerial2"));
  pms.begin(Serial2);
#else
  Serial.println(F("PMS sensor on HardwareSerial1"));
  pms.begin(Serial1);
#endif
  pms.init();
}

void loop() {
  // read the PM sensor
  pms.read();

#ifdef ESP32
  // print formatted results
  Serial.printf("PM1 %d, PM2.5 %d, PM10 %d [ug/m3]\n",
    pms.pm[0],pms.pm[1],pms.pm[2]);
#else
  // print the results
  Serial.print(F("PM1 "))  ;Serial.print(pms.pm[0]);Serial.print(F(", "));
  Serial.print(F("PM2.5 "));Serial.print(pms.pm[1]);Serial.print(F(", "));
  Serial.print(F("PM10 ")) ;Serial.print(pms.pm[2]);Serial.println(F(" [ug/m3]"));
#endif

  // wait for 10 seconds
  delay(10000);
}
