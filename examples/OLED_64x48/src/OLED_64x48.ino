// PMSerail: OLED_64x48.ino

#include <PMserial.h>  // Arduino library for PM sensors with serial interface
SerialPM pms(PMS5003); // aka G5

#ifdef ESP8266
#include <SoftwareSerial.h>
const uint8_t PMS_RX=2, PMS_TX=0;
SoftwareSerial mySerial(PMS_RX,PMS_TX); // RX, TX
#elif defined(ESP32)
#include <HardwareSerial.h>
const uint8_t PMS_RX=16, PMS_TX=17;
HardwareSerial mySerial(2);   // UART2 on GPIO16(RX),GPIO17(TX)
#endif

#include <Wire.h>
#include "SSD1306.h"
SSD1306 display(0x3c, SDA, SCL);
// this is a 64x48 display, not 128x64
const uint8_t WIDTH=64, HEIGHT=48;

void setup() {
  Serial.begin(9600);
  Serial.printf("Booted\n");

  pms.begin(mySerial);
  Serial.printf("PMS sensor on RX:GPIO%02d, TX:GPIO%02%d\n",PMS_RX,PMS_TX);
  pms.init();

  Serial.printf("%02dx%02d OLED on SDA:GPIO%02d, SCL:GPIO%02d \n",
                WIDTH, HEIGHT, SDA, SCL);
  display.init();
}

void barGraph(uint16_t *val, uint8_t nval) {
  const uint8_t i0=88;
  display.clear();
  for (int8_t i=0; i<nval; i++) {
    display.fillRect(i0-i*8, 0, 6, (i+1)*HEIGHT/8);
  }
  display.display();
}

void loop() {
  // read the PM sensor
  pms.read();

  Serial.printf("PM1 %d, PM2.5 %d, PM10 %d [ug/m3], ",
    pms.pm[0],pms.pm[1],pms.pm[2]);
  barGraph(pms.pm, 3);

  // wait for 5 seconds
  delay(5000);

  Serial.printf("N0.3 %d, N0.5 %d, N1 %d, N2.5 %d, N5 %d, N10 %d [#/100cc]\n",
    pms.nc[0],pms.nc[1],pms.nc[2],pms.nc[3],pms.nc[4],pms.nc[5]);
  barGraph(pms.nc, 6);

  // wait for 5 seconds
  delay(5000);
}
