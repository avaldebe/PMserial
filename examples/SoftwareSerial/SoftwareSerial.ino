// SoftwareSerial.ino: Read PMS5003 sensor on SWSerial

#ifndef ESP32
#include <SoftwareSerial.h>
#endif
#include <PMserial.h> // Arduino library for PM sensors with serial interface

#if !defined(PMS_RX) && !defined(PMS_TX)
constexpr auto PMS_RX = 10;
constexpr auto PMS_TX = 11;
#endif

#ifndef ESP32
SerialPM pms(PMS5003, PMS_RX, PMS_TX); // PMSx003, RX, TX

// Alternative:
//SoftwareSerial SoftSerial1(PMS_RX, PMS_TX);
//SerialPM pms(PMS5003, SoftSerial1);
#else
SerialPM pms(PMS5003, PMS_RX, PMS_TX); // PMSx003, RX, TX
#endif

void setup()
{
  Serial.begin(9600);
  Serial.println(F("Booted"));

  Serial.println(F("PMS sensor on SWSerial"));
  Serial.print(F("  RX:"));
  Serial.println(PMS_RX);
  Serial.print(F("  TX:"));
  Serial.println(PMS_TX);

  pms.init();
}

void loop()
{
  // read the PM sensor
  pms.read();
  if (pms)
  { // successfull read
#ifdef ESP8266
    // print formatted results
    Serial.printf("PM1.0 %2d, PM2.5 %2d, PM10 %2d [ug/m3]\n",
                  pms.pm01, pms.pm25, pms.pm10);

    if (pms.has_number_concentration())
      Serial.printf("N0.3 %4d, N0.5 %3d, N1.0 %2d, N2.5 %2d, N5.0 %2d, N10 %2d [#/100cc]\n",
                    pms.n0p3, pms.n0p5, pms.n1p0, pms.n2p5, pms.n5p0, pms.n10p0);

    if (pms.has_temperature_humidity() || pms.has_formaldehyde())
      Serial.printf("%5.1f °C, %5.1f %%rh, %5.2f mg/m3 HCHO\n",
                    pms.temp, pms.rhum, pms.hcho);
#else
    // print the results
    Serial.print(F("PM1.0 "));
    Serial.print(pms.pm01);
    Serial.print(F(", "));
    Serial.print(F("PM2.5 "));
    Serial.print(pms.pm25);
    Serial.print(F(", "));
    Serial.print(F("PM10 "));
    Serial.print(pms.pm10);
    Serial.println(F(" [ug/m3]"));

    if (pms.has_number_concentration())
    {
      Serial.print(F("N0.3 "));
      Serial.print(pms.n0p3);
      Serial.print(F(", "));
      Serial.print(F("N0.5 "));
      Serial.print(pms.n0p5);
      Serial.print(F(", "));
      Serial.print(F("N1.0 "));
      Serial.print(pms.n1p0);
      Serial.print(F(", "));
      Serial.print(F("N2.5 "));
      Serial.print(pms.n2p5);
      Serial.print(F(", "));
      Serial.print(F("N5.0 "));
      Serial.print(pms.n5p0);
      Serial.print(F(", "));
      Serial.print(F("N10 "));
      Serial.print(pms.n10p0);
      Serial.println(F(" [#/100cc]"));
    }

    if (pms.has_temperature_humidity() || pms.has_formaldehyde())
    {
      Serial.print(pms.temp, 1);
      Serial.print(F(" °C"));
      Serial.print(F(", "));
      Serial.print(pms.rhum, 1);
      Serial.print(F(" %rh"));
      Serial.print(F(", "));
      Serial.print(pms.hcho, 2);
      Serial.println(F(" mg/m3 HCHO"));
    }
#endif
  }
  else
  { // something went wrong
    switch (pms.status)
    {
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
