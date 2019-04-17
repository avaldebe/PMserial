// SoftwareSerial.ino: Read PMS5003 sensor on SWSerial

#include <SoftwareSerial.h>
#include <PMserial.h>  // Arduino library for PM sensors with serial interface
#if !defined(PMS_RX) && !defined(PMS_TX)
const uint8_t PMS_RX=10, PMS_TX=11;
#endif
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
  if(pms){  // successfull read
#ifdef ESP8266
    // print formatted results
    Serial.printf("PM1.0 %2d, PM2.5 %2d, PM10 %2d [ug/m3]\n",
      pms.pm01,pms.pm25,pms.pm10);
#else
    // print the results
    Serial.print(F("PM1.0 "));Serial.print(pms.pm01);Serial.print(F(", "));
    Serial.print(F("PM2.5 "));Serial.print(pms.pm25);Serial.print(F(", "));
    Serial.print(F("PM10 ")) ;Serial.print(pms.pm10);Serial.println(F(" [ug/m3]"));
#endif
  } else { // something went wrong
    switch (pms.status) {
    case pms.OK: // should never come here
      break;     // included to compile without warnings
    case pms.ERROR_TIMEOUT:
      Serial.println(F(PMS_ERROR_TIMEOUT));
      break;
    case pms.ERROR_MSG_UNKNOWN:
      Serial.println(F(PMS_ERROR_MSG_UNKNOWN));
      break;
    case pms.ERROR_MSG_HEADER:
      Serial.println(F(PMS_ERROR_MSG_HEADER));
      break;
    case pms.ERROR_MSG_BODY:
      Serial.println(F(PMS_ERROR_MSG_BODY));
      break;
    case pms.ERROR_MSG_START:
      Serial.println(F(PMS_ERROR_MSG_START));
      break;
    case pms.ERROR_MSG_LENGTH:
      Serial.println(F(PMS_ERROR_MSG_LENGTH));
      break;
    case pms.ERROR_MSG_CKSUM:
      Serial.println(F(PMS_ERROR_MSG_CKSUM));
      break;
    case pms.ERROR_PMS_TYPE:
      Serial.println(F(PMS_ERROR_PMS_TYPE));
      break;
    }
  }

  // wait for 10 seconds
  delay(10000);
}
