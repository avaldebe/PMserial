# PMserial

Arduino library for PM sensors with serial interface

## Sensors

[plantower][]      | Tested Works | Doesn't Work | Not Tested  | Notes
------------------ | :----------: | :----------: | :---------: | -----
PMS1003 (aka G1)   |   |   | X |
PMS3003 (aka G3)   |   |   | X |
PMS5003 (aka G5)   | X |   |   |
PMS7003 (aka G7)   |   |   | X |
PMSA003 (aka G10)  |   |   | X |

[plantower]:  http://www.plantower.com/

## Compatibility

MCU                | Tested Works | Doesn't Work | Not Tested  | Notes
------------------ | :----------: | :----------: | :---------: | -----
Atmega328  @  8MHz | X |   |   | [examples/SoftwareSerial][]
Atmega328  @ 16MHz |   |   | X | [examples/SoftwareSerial][]
ATmega168  @  8MHz |   |   | X | [examples/SoftwareSerial][]
Atmega168  @ 16MHz |   |   | X |
Atmega32u4 @  8MHz |   |   | X |
Atmega32u4 @ 16MHz |   |   | X | [examples/HardwareSerial1][]
Atmega2560 @ 16MHz | X |   |   | [examples/HardwareSerial1][]
STM32f103c8        |   |   | X |
STM32f103cb        |   |   | X | [examples/HardwareSerial1][]
ESP8266            | X |   |   | [examples/SoftwareSerial][]
ESP832             |   |   | X |

[examples/SoftwareSerial]: examples/SoftwareSerial/README.md
[examples/HardwareSerial1]: examples/HardwareSerial1/README.md
