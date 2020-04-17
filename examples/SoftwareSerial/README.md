# PMSerial: HardwareSerial1.ino

Read PMS5003 sensor on SWSerial

## [Arduino Uno](https://docs.platformio.org/en/latest/boards/atmelavr/uno.html)

**Note** This is a 5V board, and the PMS5003 RX/TX pins are 3.3V.
You need to add a logic level converter, or use a 3.3V version of this board.

- SWSerial on pins 10 (RX) and 11 (TX).

```ini
[env:uno]
framework = arduino
platform = atmelavr
board = uno
```

```bash
# compile
platformio run -e uno

# upload
platformio run -e uno -t upload

# open serial monitor
platformio run -e uno -t monitor
```

## [Arduino Pro Mini (ATmega168, 3.3V, 8 MHz)](https://docs.platformio.org/en/latest/boards/atmelavr/pro8MHzatmega168.html)

3.3V/8Mhz version of the pro mini board with an ATmega168.
**Note** This is a 3.3V board, and the PMS5003 requires 5V.
You need provide 5V for the fan to operate properly.

- SWSerial on pins 10 (RX) and 11 (TX).

```ini
[env:mini168_3V3]
framework = arduino
platform = atmelavr
board = pro8MHzatmega168
```

```bash
# compile
platformio run -e mini168_3V3

# upload
platformio run -e mini168_3V3 -t upload

# open serial monitor
platformio run -e mini168_3V3 -t monitor
```

## [Arduino Pro Mini (ATmega328, 3.3V, 8 MHz)](https://docs.platformio.org/en/latest/boards/atmelavr/pro8MHzatmega328.html)

3.3V/8Mhz version of the pro mini board with an ATmega328.
**Note** This is a 3.3V board, and the PMS5003 requires 5V.
You need provide 5V for the fan to operate properly.

- SWSerial on pins 10 (RX) and 11 (TX).

```ini
[env:mini328_3V3]
framework = arduino
platform = atmelavr
board = pro8MHzatmega328
```

```bash
# compile
platformio run -e mini328_3V3

# upload
platformio run -e mini328_3V3 -t upload

# open serial monitor
platformio run -e mini328_3V3 -t monitor
```

## [ESP8266 ESP-01 512k](https://docs.platformio.org/en/latest/boards/espressif8266/esp01.html)

**Note** This is a 3.3V board, and the PMS5003 requires 5V.

- SWSerial on pins GPIO2 (RX) and GPIO0 (TX).
- Needs [EspSoftwareSerial@>=6.7.1][issue6]

[issue6]: https://github.com/avaldebe/PMserial/issues/6

```ini
[env:esp01]
framework = arduino
platform = espressif8266
lib_deps = EspSoftwareSerial@>=6.7.1
board = esp01
build_flags = -D PMS_RX=2 -D PMS_TX=0
```

```bash
# compile
platformio run -e esp01

# upload
platformio run -e esp01 -t upload

# open serial monitor
platformio run -e esp01 -t monitor
```

## [WeMos D1 mini](https://docs.platformio.org/en/latest/boards/espressif8266/d1_mini.html)

**Note** This is a 3.3V board, and the PMS5003 requires 5V.

- SWSerial on pins GPIO13/D7 (RX) and GPIO12/D6 (TX).
- Needs [EspSoftwareSerial@>=6.7.1][issue6]

```ini
[env:d1_mini]
framework = arduino
platform = espressif8266
lib_deps = EspSoftwareSerial@>=6.7.1
board = d1_mini
build_flags = -D PMS_RX=D7 -D PMS_TX=D6
```

```bash
# compile
platformio run -e d1_mini

# upload
platformio run -e d1_mini -t upload

# open serial monitor
platformio run -e d1_mini -t monitor
```

## [ESP32 MiniKit](https://docs.platformio.org/en/latest/boards/espressif32/mhetesp32minikit.html)

**Note** This is a 3.3V board, and the PMS5003 requires 5V.
You need provide 5V for the fan to operate properly.

- Serial1 is by default on pins 9 (RX) and 10 (TX).
- Serial2 on pins 16 (RX) and 17 (TX).

On some ESP32 boards Serial1 default pins are connected to the flash.
Using the standard constructor will cause a crash, see [espressif/arduino-esp32#148](https://github.com/espressif/arduino-esp32/issues/148).

```c++
// will crash the ESP32
SerialPM pms(PMSx003, Serial1);
```

Fortunately, it is possible to define alternative for pins by calling:

```c++
// define Serial1 pins
Serial1.begin(9600, SERIAL_8N1, <RX>, <TX>);
```

The PMSerial library uses this feature to implement the flexibility of SoftwareSerial

```c++
// define Serial1 pins
SerialPM pms(PMS5003, <RX>, <TX>);
```

This example uses Serial1 on pins 23 (RX) and 19 (TX).
The [HardwareSerial example][esp32hw] uses Serial2 directly.

[esp32hw]: ../HardwareSerial/README.md#esp32-minikit

```ini
[env:esp32minikit]
framework = arduino
platform = espressif32
board = mhetesp32minikit
build_flags = -D PMS_RX=23 -D PMS_TX=19
```

```bash
# compile
platformio run -e esp32minikit

# upload
platformio run -e esp32minikit -t upload

# open serial monitor
platformio run -e esp32minikit -t monitor
```
