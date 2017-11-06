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
const uint8_t WIDTH=64, HEIGHT=48, X1=WIDTH/2, X0=DISPLAY_WIDTH-X1;

#define DEBUG

void setup() {
  Serial.begin(9600);
  Serial.printf("Booted\n");

  pms.begin(mySerial);
  Serial.printf("PMS sensor on RX:GPIO%02d, TX:GPIO%02%d\n",PMS_RX,PMS_TX);
  pms.init();

  Serial.printf("%02dx%02d OLED on SDA:GPIO%02d, SCL:GPIO%02d \n",
                WIDTH, HEIGHT, SDA, SCL);
  display.init();
#ifdef DEBUG
  display.invertDisplay(); // debug positions
#endif
}

void barPM(uint16_t *pm){
  const uint8_t barWidth=8, nbar=WIDTH/barWidth, nbin=3, x0=X0-barWidth;
  static uint8_t buffer[nbar*nbin], pos=0, scale=0;
  uint8_t n, bin, bar;

  // rescale buffer, if necesary
  uint8_t rescale=0;
  while ((pm[nbin-1]>>scale)>HEIGHT) {
    scale++;
    rescale++;
#ifdef DEBUG
    Serial.printf("\n  scale:%d, rescale:%d",scale,rescale);
#endif
  }
  if (rescale>0) {
    for (n=0; n<nbar*nbin; n++) {
      buffer[n] >>= rescale;
    }
  }
 
  // buffer new value
  pos%=nbar; // ensure we are inside the buffer
  for (bin=0, n=pos*nbin; bin<nbin; bin++, n++){
     buffer[n] = pm[bin]>>scale;
  }
  pos++;     // next possition on the buffer, first to plot

  // plot buffer
  display.clear();
  for (n=0; n<nbar*nbin; n++){
    bar=(nbar-pos+n/nbin)%nbar;
    display.drawRect(x0-barWidth*bar, 0, barWidth-2, buffer[n]);
#ifdef DEBUG
    if(bar == nbar-1)
      Serial.printf("\n  pos:%d, bar:%d, val:%d",pos,bar,buffer[n]);
    display.display();
    delay(50);
#endif
  }
  display.display();

}

void barNC(uint16_t *nc){
  const uint8_t nbin=6, barWidth=WIDTH/nbin, x0=X0-barWidth;
  uint8_t bin, scale;

  // rescale input, if necesary
  scale=0;
  for (bin=0; bin<nbin; bin++){
    while ((nc[bin]>>scale)>HEIGHT) {
      scale++;
    }
  }

  // plot input
  display.clear();
  for (bin=0; bin<nbin; bin++){
    display.fillRect(x0-barWidth*bin, 0, barWidth-2, nc[bin]>>scale);
#ifdef DEBUG
    display.display();
    delay(150);
#endif
  }
  display.display();
}

void barGraph(uint16_t *val, uint8_t nval) {
  const uint8_t i0=DISPLAY_WIDTH-WIDTH/2;
  display.clear();
  for (int8_t i=1; i<=nval; i++) {
    display.fillRect(i0-i*8, 0, 6, i*HEIGHT/8);
  }
  display.display();
}

void loop() {
#ifdef DEBUG
 static uint16_t pm[3]={0,1,2}, nc[6]={0,0x0FFF,0x0F00,0x00FF,0x00F0,0};
  barPM(pm);
  barNC(nc);
  pm[0]++;
  pm[1]+=2;
  pm[2]+=4;

#else
  // read the PM sensor
  pms.read();

  // print/display PM values
  Serial.printf("PM1 %d, PM2.5 %d, PM10 %d [ug/m3], ",
    pms.pm[0],pms.pm[1],pms.pm[2]);
  barPM(pms.pm);

  // wait for 5 seconds
  delay(5000);

  // print/display Number/Count values
  Serial.printf("N0.3 %d, N0.5 %d, N1 %d, N2.5 %d, N5 %d, N10 %d [#/100cc]\n",
    pms.nc[0],pms.nc[1],pms.nc[2],pms.nc[3],pms.nc[4],pms.nc[5]);
  barGraph(pms.nc, 6);


  // wait for 5 seconds
  delay(5000);
#endif
}
