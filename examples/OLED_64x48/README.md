# PMSerial: OLED_64x48.ino

Read PMS5003 sensor and display results on a 0.66 inch OLED.

## OLED display

In particular this example uses the [WEMOS OLED shield][oled_shield],
and the [esp8266-oled-ssd1306][OLEDlib] library.

```bash
# install the libraries (one time setup)
platformio lib install "thingpulse/ESP8266 and ESP32 OLED driver for SSD1306 displays"
```

[oled_shield]: https://wiki.wemos.cc/products:d1_mini_shields:oled_shield
[OLEDlib]: https://github.com/ThingPulse/esp8266-oled-ssd1306

## ESP8266 `d1_mini`

**Note** This is a 3.3V board, and the PMS5003 requires 5V.

- SoftwareSerial on pins gpio2/D4 (RX) and gpio0/D3 (TX).
- OLED shield is on pins gpio4/D3 (SDA) and gpio5/D5 (SCL).

```bash
# compile
platformio run -e d1_mini

# upload
platformio run -e d1_mini -t upload

# open serial monitor
platformio run -e d1_mini -t monitor
```

## ESP32 `esp32minikit`

**Note** This is a 3.3V board, and the PMS5003 requires 5V.
You need provide 5V for the fan to operate properly.

- Serial2 on pins gpio16 (RX) and gpio17 (TX).
- OLED shield is on pins gpio21 (SDA) and gpio22 (SCL).

```bash
# compile
platformio run -e esp32minikit

# upload
platformio run -e esp32minikit -t upload

# open serial monitor
platformio run -e esp32minikit -t monitor
```
