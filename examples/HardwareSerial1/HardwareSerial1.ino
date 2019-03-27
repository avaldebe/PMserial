// HardwareSerial1.ino: Read PMS5003 sensor on Serial1

#include <PMserial.h>  // Arduino library for PM sensors with serial interface
#ifdef ESP32
  #define MSG "PMS5003 on HardwareSerial2"
  SerialPM pms(PMS5003, Serial2);         // PMSx003, UART
#else
  #define MSG "PMS5003 on HardwareSerial1"
  SerialPM pms(PMS5003, Serial1);         // PMSx003, UART
#endif

void setup() {
  Serial.begin(9600);
  Serial.println(F("Booted"));

  Serial.println(F(MSG));
  pms.init();
}

void loop() {
  // read the PM sensor
  pms.read();
  if(pms){  // sucessfull read
#ifdef ESP32
    // print formatted results
    Serial.printf("PM1.0 %d, PM2.5 %d, PM10 %d [ug/m3]\n",
      pms.pm01,pms.pm25,pms.pm10);
#else
    // print the results
    Serial.print(F("PM1.0 "));Serial.print(pms.pm01);Serial.print(F(", "));
    Serial.print(F("PM2.5 "));Serial.print(pms.pm25);Serial.print(F(", "));
    Serial.print(F("PM10 ")) ;Serial.print(pms.pm10);Serial.println(F(" [ug/m3]"));
#endif
  } else { // something went wrong
    switch (pms.status) {
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
    case pms.ERROR_MSG_LENGHT:
      Serial.println(F(PMS_ERROR_MSG_LENGHT));
      break;
    case pms.ERROR_MSG_CKSUM:
      Serial.println(F(PMS_ERROR_MSG_CKSUM));
      break;
    case pms.ERROR_PMS_TYPE:
      Serial.println(F(PMS_ERROR_MSG_CKSUM));
      break;
    }
  }

  // wait for 10 seconds
  delay(10000);
}
