/* SerialPM
 Arduino library for PM sensors with serial interface
  PMS1003 aka G1
  PMS3003 aka G2
  PMS5003 aka G5
  PMS7003 aka G7
  PMSA003 aka G10
*/
#include <Arduino.h>

#define PMS_DEBUG
#include <PMserial.h>
#if defined(USE_HWSERIAL2)
#define MSG "PMSx003 on HardwareSerial2"
SerialPM pms(PMSx003, Serial2); // PMSx003, UART
#elif defined(USE_HWSERIAL1)
#define MSG "PMSx003 on HardwareSerial1"
SerialPM pms(PMSx003, Serial1); // PMSx003, UART
#elif defined(USE_HWSERIAL)
#define MSG "PMSx003 on HardwareSerial"
SerialPM pms(PMSx003, Serial); // PMSx003, UART
#elif defined(PMS_RX) && defined(PMS_TX)
#ifdef ESP32
#define MSG "PMSx003 on HardwareSerial1"
#else
#define MSG "PMSx003 on SoftwareSerial"
#endif
SerialPM pms(PMSx003, PMS_RX, PMS_TX); // PMSx003, RX, TX
#elif defined(ESP32)
#define MSG "PMSx003 on HardwareSerial2"
SerialPM pms(PMSx003, Serial2); // PMSx003, UART
#elif defined(HAS_HW_SERIAL1)
#define MSG "PMSx003 on HardwareSerial1"
SerialPM pms(PMSx003, Serial1); // PMSx003, UART
#elif defined(HAS_SW_SERIAL)
#define MSG "PMSx003 on SoftwareSerial"
SerialPM pms(PMSx003, 10, 11); // PMSx003, RX, TX
#else
#define MSG "PMSx003 on HardwareSerial"
SerialPM pms(PMSx003, Serial); // PMSx003, UART
#endif

void setup()
{
  Serial.begin(9600);
  Serial.println(F("Booted"));

  Serial.println(F(MSG));
  pms.init();
}

void printPMS()
{
#if defined(ESP8266) || defined(ESP8266)
  if (!pms.has_particulate_matter())
    return;
  Serial.printf("PM1.0 %2d, PM2.5 %2d, PM10 %2d [ug/m3]\n",
                pms.pm01, pms.pm25, pms.pm10);

  if (!pms.has_number_concentration())
    return;
  Serial.printf("N0.3 %4d, N0.5 %3d, N1.0 %2d, N2.5 %2d, N5.0 %2d, N10 %2d [#/100cc]\n",
                pms.n0p3, pms.n0p5, pms.n1p0, pms.n2p5, pms.n5p0, pms.n10p0);

  if (!pms.has_temperature_humidity() && !pms.has_formaldehyde())
    return;
  Serial.printf("%5.1f °C, %5.1f %%rh, %5.2f mg/m3 HCHO\n",
                pms.temp, pms.rhum, pms.hcho);
#else
  if (!pms.has_particulate_matter())
    return;
  Serial.print(F("PM1.0 "));
  Serial.print(pms.pm01);
  Serial.print(F(", "));
  Serial.print(F("PM2.5 "));
  Serial.print(pms.pm25);
  Serial.print(F(", "));
  Serial.print(F("PM10 "));
  Serial.print(pms.pm10);
  Serial.println(F(" [ug/m3]"));

  if (!pms.has_number_concentration())
    return;
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

  if (!pms.has_temperature_humidity() && !pms.has_formaldehyde())
    return;
  Serial.print(pms.temp, 1);
  Serial.print(F(" °C"));
  Serial.print(F(", "));
  Serial.print(pms.rhum, 1);
  Serial.print(F(" %rh"));
  Serial.print(F(", "));
  Serial.print(pms.hcho, 2);
  Serial.println(F(" mg/m3 HCHO"));
#endif
}

void statusPMS()
{
  pms.print_buffer(Serial, " %04X");
  static uint16_t readings = 0, errors = 0;
  readings++;
  if (!pms)
    errors++;
#if defined(ESP8266) || defined(ESP32)
  Serial.printf("\nErrors %d/%d, %d bytes read, %d ms waited\n", errors, readings, pms.bytes_read(), pms.waited_ms());
#else
  Serial.print(F("\nErrors "));
  Serial.print(errors);
  Serial.print(F("/"));
  Serial.print(readings);
  Serial.print(F(", "));
  Serial.println(pms.bytes_read());
  Serial.print(F(" bytes read"));
  Serial.print(F(", "));
  Serial.println(pms.waited_ms());
  Serial.println(F(" ms waited"));
#endif
  switch (pms.status)
  {
  case pms.OK:
    printPMS();
    break;
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
  default:
    Serial.println(F("Unknown error"));
    break;
  }
}

void loop()
{
  pms.read();   // read the PM sensor
  statusPMS();  // print result/error
  delay(10000); // wait for 10 seconds
}
