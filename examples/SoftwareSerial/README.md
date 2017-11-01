# PMSerail: HardwareSerial1.ino

Read PMS5003 sensor on SWSerial

## Arduino `uno`

**Note** This is a 5V board, and the PMS5003 RX/TX pins are 3.3V.
You need to add a logic level converter, or use a 3.3V version of this board.

- SWSerial on pins 10 (RX) and 11 (TX).

```bash
# compile
platformio run -e uno

# upload
platformio run -e uno -t upload

# open serial monitor
platformio run -e uno -t monitor
```

## Arduino `mini168_3V3`

3.3V/8Mhz version of the pro mini board with an ATmega168.
**Note** This is a 3.3V board, and the PMS5003 requires 5V.
You need provide 5V for the fan to operate properly.

- SWSerial on pins 10 (RX) and 11 (TX).

```bash
# compile
platformio run -e mini168_3V3

# upload
platformio run -e mini168_3V3 -t upload

# open serial monitor
platformio run -e mini168_3V3 -t monitor
```

## Arduino `mini328_3V3`

3.3V/8Mhz version of the pro mini board with an ATmega328.
**Note** This is a 3.3V board, and the PMS5003 requires 5V.
You need provide 5V for the fan to operate properly.

- SWSerial on pins 10 (RX) and 11 (TX).

```bash
# compile
platformio run -e mini328_3V3

# upload
platformio run -e mini328_3V3 -t upload

# open serial monitor
platformio run -e mini328_3V3 -t monitor
```

## ESP8266 `esp01`

**Note** This is a 3.3V board, and the PMS5003 requires 5V.

- SWSerial on pins GPIO2 (RX) and GPIO0 (TX).

```bash
# compile
platformio run -e esp01

# upload
platformio run -e esp01 -t upload

# open serial monitor
platformio run -e esp01 -t monitor
```

## ESP8266 `d1_mini`

**Note** This is a 3.3V board, and the PMS5003 requires 5V.

- SWSerial on pins GPIO2/D4 (RX) and GPIO0/D3 (TX).

```bash
# compile
platformio run -e d1_mini

# upload
platformio run -e d1_mini -t upload

# open serial monitor
platformio run -e d1_mini -t monitor
```
