// HardwareSerial1.ino: Read PMS5003 sensor on Serial1

#include <PMserial.h>  // Arduino library for PM sensors with serial interface
SerialPM pms(PMS5003); // aka G5

void setup() {
  Serial.begin(9600);
  Serial.println("Booted");

  Serial.println("PMS sensor on HardwareSerial1");
  pms.begin(Serial1);
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
