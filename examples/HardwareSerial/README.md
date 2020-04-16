# PMSerial: HardwareSerial.ino

Read PMS5003 sensor on Serial/Serial1/Serial2, depending on the board.

## Arduino `leonardo`

**Note** This is a 5V board, and the PMS5003 RX/TX pins are 3.3V.
You need to add a logic level converter, or use a 3.3V version of this board.

- Serial1 on pins 0 (RX) and 1 (TX).

```bash
# compile
platformio run -e leonardo

# upload
platformio run -e leonardo -t upload

# open serial monitor
platformio run -e leonardo -t monitor
```

## Arduino `mega2560`

**Note** This is a 5V board, and the PMS5003 RX/TX pins are 3.3V.
You need to add a logic level converter, or use a 3.3V version of this board.

- Serial1 on pins 19 (RX) and 18 (TX),
- Serial2 on pins 17 (RX) and 16 (TX),
- Serial3 on pins 15 (RX) and 14 (TX).

This example uses Serial1.

```bash
# compile
platformio run -e mega2560

# upload
platformio run -e mega2560 -t upload

# open serial monitor
platformio run -e mega2560 -t monitor
```

## STM32 `genericSTM32F103C8`

**Note** This is a 3.3V board, and the PMS5003 requires 5V.
You need provide 5V for the fan to operate properly.

- Serial1 on pins PA10 (RX) and PA9 (TX),
- Serial2 on pins PA3  (RX) and PA2 (TX),
- Serial3 on pins PB11 (RX) and PB10 (TX).

This example uses Serial1.

```bash
# compile
platformio run -e genericSTM32F103C8

# upload
platformio run -e genericSTM32F103C8 -t upload

# open serial monitor
platformio run -e genericSTM32F103C8 -t monitor
```

## STM32 `maple_mini`

**Note** This is a 3.3V board, and the PMS5003 requires 5V.
You need provide 5V for the fan to operate properly.

- Serial1 on pins 25/PA10 (RX) and 26/PA9 (TX),
- Serial2 on pins 8/PA3  (RX) and 9/PA2 (TX),
- Serial3 on pins 0/PB11 (RX) and 1/PB10 (TX).

This example uses Serial2.

```bash
# compile
platformio run -e maple_mini

# upload
platformio run -e maple_mini -t upload

# open serial monitor
platformio run -e maple_mini -t monitor
```

## ESP8266 `esp01`

**Note** This is a 3.3V board, and the PMS5003 requires 5V.

- Serial is the only HardwareSerial.

This example used Serial for communicating with the sensor
and printing the sensor values.
Some garbage between printouts is to be expected.

```bash
# compile
platformio run -e esp01

# upload
platformio run -e esp01 -t upload

# open serial monitor
platformio run -e esp01 -t monitor
```

## ESP32 `esp32minikit`

**Note** This is a 3.3V board, and the PMS5003 requires 5V.
You need provide 5V for the fan to operate properly.

- Serial1 is by default on pins 9 (RX) and 10 (TX).
- Serial2 on pins 16 (RX) and 17 (TX).

On some ESP32 boards Serial1 default pins are connected to the flash.
Using the standard constructor will cause a crash, see [espressif/arduino-esp32#148](https://github.com/espressif/arduino-esp32/issues/148).

```Arduino
// will crash the ESP32
SerialPM pms(PMSx003, Serial1);
````

Fortunately, it is possible to define alternative for pins by calling:

```Arduino
// define Serial1 pins
Serial1.begin(9600, SERIAL_8N1, <RX>, <TX>);
```

The PMSerial library uses this feature to implement the flexibility of SoftwareSerial

```Arduino
// define Serial1 pins
SerialPM pms(PMS5003, <RX>, <TX>);
````

The [SoftwareSerial example][esp32sw] uses Serial1 on pins 16 (RX) and 17 (TX).
This example uses Serial2 directly.

[esp32sw]: ../SoftwareSerial/README.md#esp32-esp32minikit


```bash
# compile
platformio run -e esp32minikit

# upload
platformio run -e esp32minikit -t upload

# open serial monitor
platformio run -e esp32minikit -t monitor
```
