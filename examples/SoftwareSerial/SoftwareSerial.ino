// SoftwareSerial.ino: Read PMS5003 sensor on SWSerial

#include <SoftwareSerial.h>
#ifdef ESP8266
const uint8_t PMS_RX=2, PMS_TX=0;
#else
const uint8_t PMS_RX=10, PMS_TX=11;
#endif
#include <PMserial.h>  // Arduino library for PM sensors with serial interface
SerialPM pms(PMS5003, PMS_RX, PMS_TX);  // PMSx003, RX, TX

void setup() {
  Serial.begin(9600);
  Serial.println(F("Booted"));

  Serial.println(F("PMS sensor on SWSerial"));
  Serial.print(F("  RX:"));Serial.println(PMS_RX);
  Serial.print(F("  TX:"));Serial.println(PMS_TX);
  pms.init();
}

void loop() {
  // read the PM sensor
  pms.read();

#ifdef ESP8266
  // print formatted results
  Serial.printf("PM1 %d, PM2.5 %d, PM10 %d [ug/m3]\n",
    pms.pm01,pms.pm25,pms.pm10);
#else
  // print the results
  Serial.print(F("PM1 "))  ;Serial.print(pms.pm01);Serial.print(F(", "));
  Serial.print(F("PM2.5 "));Serial.print(pms.pm25);Serial.print(F(", "));
  Serial.print(F("PM10 ")) ;Serial.print(pms.pm10);Serial.println(F(" [ug/m3]"));
#endif

  // wait for 10 seconds
  delay(10000);
}
