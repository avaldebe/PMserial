// SoftwareSerial.ino: Read PMS5003 sensor on SWSerial

#include <SoftwareSerial.h>
#ifdef ESP8266
SoftwareSerial SWSerial(2,0); // RX, TX
#else
SoftwareSerial SWSerial(10,11); // RX, TX
#endif

#include <PMserial.h>  // Arduino library for PM sensors with serial interface
SerialPM pms(PMS5003); // aka G5

void setup() {
  Serial.begin(9600);
  Serial.println(F("Booted"));

  Serial.println(F("PMS sensor on SWSerial"));
  pms.begin(SWSerial);
  pms.init();
}

void loop() {
  // read the PM sensor
  pms.read();

#ifdef ESP8266
  // print formatted results
  Serial.printf("PM1 %d, PM2.5 %d, PM10 %d [ug/m3]\n",
    pms.pm_atm[0],pms.pm_atm[1],pms.pm_atm[2]);
#else
  // print the results
  Serial.print(F("PM1 "))  ;Serial.print(pms.pm_atm[0]);Serial.print(F(", "));
  Serial.print(F("PM2.5 "));Serial.print(pms.pm_atm[1]);Serial.print(F(", "));
  Serial.print(F("PM10 ")) ;Serial.print(pms.pm_atm[2]);Serial.println(F(" [ug/m3]"));
#endif

  // wait for 10 seconds
  delay(10000);
}
