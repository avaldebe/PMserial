// PMSerail: OLED_64x48.ino
#include <PMserial.h> // Arduino library for PM sensors with serial interface

#ifdef ESP8266
#include <SoftwareSerial.h>
const uint8_t PMS_RX = 2, PMS_TX = 0;
SerialPM pms(PMS5003, PMS_RX, PMS_TX); // PMSx003, RX, TX
#elif defined(ESP32)
const uint8_t PMS_RX = 16, PMS_TX = 17;
SerialPM pms(PMS5003, Serial2); // PMSx003, UART
#else
#error "Unknown MCU"
#endif

#include <Wire.h>
#include "SSD1306.h"
SSD1306 display(0x3c, SDA, SCL);
// this is a 64x48 display, not 128x64
const uint8_t
    WIDTH = 64,
    HEIGHT = 48,
    X0 = WIDTH / 2, X1 = WIDTH - X0, XM = (X0 + X1) / 2,
    Y1 = HEIGHT, Y0 = Y1 - HEIGHT, YM = (Y0 + Y1) / 2;

//#define DEBUG

void setup()
{
  Serial.begin(9600);
  Serial.printf("\nProgram: %s\n", __FILE__);
  Serial.printf("Build: %s %s\n", __DATE__, __TIME__);

  Serial.printf("PMS5003 on RX:GPIO%02d, TX:GPIO%02d\n",
                PMS_RX, PMS_TX);
  pms.init();

  Serial.printf("%02dx%02d OLED on SDA:GPIO%02d, SCL:GPIO%02d \n",
                WIDTH, HEIGHT, SDA, SCL);
  display.init();
#ifdef DEBUG
  display.invertDisplay(); // debug positions
#endif
  display.flipScreenVertically();
}

void plotPM(uint16_t *pm, const uint16_t msec)
{
  const uint8_t barWidth = 8, nbar = WIDTH / barWidth, nbin = 3;
  static uint8_t buffer[nbar * nbin], pos = 0, scale = 0;
  uint8_t n, bin, bar;

  // rescale buffer, if necessary
  uint8_t rescale = 0;
  while ((pm[nbin - 1] >> scale) > HEIGHT)
  {
    scale++;
    rescale++;
#ifdef DEBUG
    Serial.printf("\n  scale:%d, rescale:%d", scale, rescale);
#endif
  }
  if (rescale > 0)
  {
    for (n = 0; n < nbar * nbin; n++)
    {
      buffer[n] >>= rescale;
    }
  }

  // buffer new value
  pos %= nbar; // ensure we are inside the buffer
  for (bin = 0, n = pos * nbin; bin < nbin; bin++, n++)
  {
    buffer[n] = pm[bin] >> scale;
  }
  pos++; // next position on the buffer, first to plot

  // plot buffer
  display.clear();
  for (n = 0; n < nbar * nbin; n++)
  {
    bar = (nbar - pos + n / nbin) % nbar;
    display.drawRect(X0 + barWidth * bar, Y1 - buffer[n], barWidth - 2, buffer[n]);
#ifdef DEBUG
    if (bar == nbar - 1)
      Serial.printf("\n  pos:%d, bar:%d, val:%d", pos, bar, buffer[n]);
    display.display();
    delay(50);
#endif
  }
  display.display();
  delay(msec);
}
void plotNC(uint16_t *nc, const uint16_t msec)
{
  const uint8_t nbin = 6, barWidth = WIDTH / nbin;
  uint8_t bin, scale, barHeight;

  // rescale input, if necessary
  scale = 0;
  for (bin = 0; bin < nbin; bin++)
  {
    while ((nc[bin] >> scale) > HEIGHT)
    {
      scale++;
    }
  }

  // plot input
  display.clear();
  for (bin = 0; bin < nbin; bin++)
  {
    barHeight = nc[bin] >> scale;
    display.fillRect(X0 + barWidth * bin, Y1 - barHeight, barWidth - 2, barHeight);
#ifdef DEBUG
    display.display();
    delay(150);
#endif
  }
  display.display();
  delay(msec);
}

static char *uint2str(const char *fmt, uint16_t var)
{
  // wrap sprintf as a function with a pre-deffined buffer
  const uint8_t chlen = 32;
  static char buffer[chlen];
  sprintf(buffer, fmt, var);
  return buffer;
}

void showPM(uint16_t *pm, const uint16_t msec)
{
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(XM, 14, "[ug/m3]");
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(X0, 24, "PM1.0");
  display.drawString(X0, 34, "PM2.5");
  display.drawString(X0, 44, "PM10");
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(X1, 24, uint2str("%7d", pm[0])); //PM1.0
  display.drawString(X1, 34, uint2str("%7d", pm[1])); //PM2.5
  display.drawString(X1, 44, uint2str("%7d", pm[2])); //PM10
  display.display();
  delay(msec);
}

void showNC(uint16_t *nc, const uint16_t msec)
{
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(XM, 14, "[#/100cc]");
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(X0, 24, "N0.3");
  //display.drawString(X0, 34, "N0.5");
  display.drawString(X0, 34, "N1.0");
  display.drawString(X0, 44, "N2.5");
  //display.drawString(X0, 54, "N5.0");
  display.drawString(X0, 54, "N10");
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(X1, 24, uint2str("%7d", nc[0])); //N0.3
                                                      //display.drawString(X1, 34, uint2str("%7d",nc[1]));//N0.5
  display.drawString(X1, 34, uint2str("%7d", nc[2])); //N1.0
  display.drawString(X1, 44, uint2str("%7d", nc[3])); //N2.5
                                                      //display.drawString(X1, 54, uint2str("%7d",nc[4]));//N5.0
  display.drawString(X1, 54, uint2str("%7d", nc[5])); //N10
  display.display();
  delay(msec);
}

void loop()
{
  // read the PM sensor
  pms.read();
  if (pms)
  { // successfull read
    // print values
    Serial.printf("PM1.0 %2d, PM2.5 %2d, PM10 %2d [ug/m3]; ",
                  pms.pm01, pms.pm25, pms.pm10);
    Serial.printf("N0.3 %4d, N0.5 %3d, N1.0 %2d, N2.5 %2d, N5.0 %2d, N10 %2d [#/100cc]\n",
                  pms.n0p3, pms.n0p5, pms.n1p0, pms.n2p5, pms.n5p0, pms.n10p0);

    // display values
    showPM(pms.pm, 5000); // particulate matter, wait 5 sec
    plotPM(pms.pm, 5000);
    showNC(pms.nc, 5000); // number concentration, wait 5 sec
    plotNC(pms.nc, 5000);
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
}
