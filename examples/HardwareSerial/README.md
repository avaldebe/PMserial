# PMSerial: HardwareSerial.ino

Read PMS5003 sensor on Serial/Serial1/Serial2, depending on the board.

## [Arduino Leonardo](https://docs.platformio.org/en/latest/boards/atmelavr/leonardo.html)

**Note** This is a 5V board, and the PMS5003 RX/TX pins are 3.3V.
You need to add a logic level converter, or use a 3.3V version of this board.

- Serial1 on pins 0 (RX) and 1 (TX).

```ini
[env:leonardo]
framework = arduino
platform = atmelavr
board = leonardo
build_flags = -D USE_HWSERIAL1
```

```bash
# compile
platformio run -e leonardo

# upload
platformio run -e leonardo -t upload

# open serial monitor
platformio run -e leonardo -t monitor
```

## [Arduino Mega 2560](https://docs.platformio.org/en/latest/boards/atmelavr/megaatmega2560.html)

**Note** This is a 5V board, and the PMS5003 RX/TX pins are 3.3V.
You need to add a logic level converter, or use a 3.3V version of this board.

- Serial1 on pins 19 (RX) and 18 (TX),
- Serial2 on pins 17 (RX) and 16 (TX),
- Serial3 on pins 15 (RX) and 14 (TX).

This example uses Serial1.

```ini
[env:mega2560]
framework = arduino
platform = atmelavr
board = megaatmega2560
build_flags = -D USE_HWSERIAL1
```

```bash
# compile
platformio run -e mega2560

# upload
platformio run -e mega2560 -t upload

# open serial monitor
platformio run -e mega2560 -t monitor
```

## [STM32F103C8](https://docs.platformio.org/en/latest/boards/ststm32/genericSTM32F103C8.html)

**Note** This is a 3.3V board, and the PMS5003 requires 5V.
You need provide 5V for the fan to operate properly.

- Serial1 on pins PA10 (RX) and PA9 (TX),
- Serial2 on pins PA3  (RX) and PA2 (TX),
- Serial3 on pins PB11 (RX) and PB10 (TX).

This example uses Serial1.

```ini
[env:genericSTM32F103C8]
framework = arduino
platform = ststm32
board = genericSTM32F103C8
upload_protocol = stlink
build_flags = -D ENABLE_HWSERIAL1 -D USE_HWSERIAL1
```

```bash
# compile
platformio run -e genericSTM32F103C8

# upload
platformio run -e genericSTM32F103C8 -t upload

# open serial monitor
platformio run -e genericSTM32F103C8 -t monitor
```

## [STM32 Maple Mini](https://docs.platformio.org/en/latest/boards/ststm32/maple_mini_b20.html)

**Note** This is a 3.3V board, and the PMS5003 requires 5V.
You need provide 5V for the fan to operate properly.

- Serial1 on pins 25/PA10 (RX) and 26/PA9 (TX),
- Serial2 on pins 8/PA3  (RX) and 9/PA2 (TX),
- Serial3 on pins 0/PB11 (RX) and 1/PB10 (TX).

This example uses Serial2.

```ini
[env:maple_mini]
framework = arduino
platform = ststm32
board = maple_mini_b20
build_flags = -D USE_HWSERIAL2
```

```bash
# compile
platformio run -e maple_mini

# upload
platformio run -e maple_mini -t upload

# open serial monitor
platformio run -e maple_mini -t monitor
```

## [ESP8266 ESP-01 512k](https://docs.platformio.org/en/latest/boards/espressif8266/esp01.html)

**Note** This is a 3.3V board, and the PMS5003 requires 5V.

- Serial is the only HardwareSerial.

This example used Serial for communicating with the sensor
and printing the sensor values.
Some garbage between printouts is to be expected.

```ini
[env:esp01]
framework = arduino
platform = espressif8266
board = esp01
```

```bash
# compile
platformio run -e esp01

# upload
platformio run -e esp01 -t upload

# open serial monitor
platformio run -e esp01 -t monitor
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

The [SoftwareSerial example][esp32sw] uses Serial1 on pins 23 (RX) and 19 (TX).
This example uses Serial2.

[esp32sw]: ../SoftwareSerial/README.md#esp32-minikit

```ini
[env:esp32minikit]
framework = arduino
platform = espressif32
board = mhetesp32minikit
build_flags = -D USE_HWSERIAL2
```

```bash
# compile
platformio run -e esp32minikit

# upload
platformio run -e esp32minikit -t upload

# open serial monitor
platformio run -e esp32minikit -t monitor
```

## [Arduino MKR WiFi 1010](https://docs.platformio.org/en/latest/boards/atmelsam/mkrwifi1010.html)

**Note** This is a 3.3V board, and the PMS5003 requires 5V.
You need provide 5V for the fan to operate properly.

- Serial1 on pins 13 (RX) and 14 (TX).

```ini
[env:mkrwifi1010]
framework = arduino
platform = atmelsam
board = mkrwifi1010
build_flags = -D USE_HWSERIAL1
```

```bash
# compile
platformio run -e mkrwifi1010

# upload
platformio run -e mkrwifi1010 -t upload

# open serial monitor
platformio run -e mkrwifi1010 -t monitor
```
