// SoftwareSerial.ino: Read PMS5003 sensor on SWSerial

#include <SoftwareSerial.h>
#ifdef ESP8266
SoftwareSerial SWSerial(D1,D2); // RX, TX
#else
SoftwareSerial SWSerial(10,11); // RX, TX
#endif

#include <PMserial.h>  // Arduino library for PM sensors with serial interface
SerialPM pms(PMS5003); // aka G5

void setup() {
  Serial.begin(9600);
  Serial.println("Booted");

  Serial.println("PMS sensor on SWSerial");
  pms.begin(SWSerial);
  pms.init();
}

void loop() {
  // read the PM sensor
  pms.read();

  // print the results
  Serial.print("PM1 ")  ;Serial.print(pms.pm_atm[0]);Serial.print(", ");
  Serial.print("PM2.5 ");Serial.print(pms.pm_atm[1]);Serial.print(", ");
  Serial.print("PM10 ") ;Serial.print(pms.pm_atm[2]);Serial.println(" [ug/m3]");

  // wait for 10 seconds
  delay(10000);
}
