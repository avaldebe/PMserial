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
SerialPM pms(Serial,PMS1003);

void setup()
{  
  Serial.begin(9600,SERIAL_8N1);
  pms.init();
}

void loop()
{
  // read the PM sensor 
  pms.read();

  // wait for 10 seconds
  delay(10000);
}
