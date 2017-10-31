# PMSerail: HardwareSerial1.ino

Read PMS5003 sensor on Serial1

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

```bash
# compile
platformio run -e mega2560

# upload
platformio run -e mega2560 -t upload

# open serial monitor
platformio run -e mega2560 -t monitor
```

## STM32 `maple_mini`

**Note** This is a 3.3V board, and the PMS5003 requires 5V.
You need provide 5V for the fan to operate properly.

- Serial1 on pins 25/PA10 (RX) and 26/PA9 (TX),
- Serial2 on pins 8/PA3  (RX) and 9/PA2 (TX),
- Serial3 on pins 0/PB11 (RX) and 1/PB10 (TX).

```bash
# compile
platformio run -e maple_mini

# upload
platformio run -e maple_mini -t upload

# open serial monitor
platformio run -e maple_mini -t monitor
```
