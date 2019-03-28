# PMSerial

Arduino library for PM sensors with serial interface

## Sensors

[plantower][]      | Tested Works | Doesn't Work | Not Tested  | Datasheet | Notes
------------------ | :----------: | :----------: | :---------: | --------- | -----
PMS1003 (aka G1)   |   |   | X | [en][g1aqmd],  [cn][g1lcsc] |
PMS3003 (aka G3)   | X |   |   | [en][g3aqmon], [cn][g3lcsc] |
PMS5003 (aka G5)   | X |   |   | [en][g5aqmd],  [cn][g5lcsc] |
PMS7003 (aka G7)   |   |   | X |                [cn][g7lcsc] |
PMSA003 (aka G10)  |   |   | X |                [cn][gAlcsc] |

[plantower]: http://www.plantower.com/
[g1aqmd]:    http://www.aqmd.gov/docs/default-source/aq-spec/resources-page/plantower-pms1003-manual_v2-5.pdf?sfvrsn=2
[g5aqmd]:    http://www.aqmd.gov/docs/default-source/aq-spec/resources-page/plantower-pms5003-manual_v2-3.pdf?sfvrsn=2
[g3aqmon]:   https://github.com/avaldebe/AQmon/raw/master/Documents/PMS3003_LOGOELE.pdf
[g5aqmon]:   https://github.com/avaldebe/AQmon/raw/master/Documents/PMS5003_LOGOELE.pdf
[g1lcsc]:    https://datasheet.lcsc.com/szlcsc/PMS1003_C89289.pdf
[g3lcsc]:    https://datasheet.lcsc.com/szlcsc/PMS3003_C87024.pdf
[g5lcsc]:    https://datasheet.lcsc.com/szlcsc/PMS5003_C91431.pdf
[g7lcsc]:    https://datasheet.lcsc.com/szlcsc/PMS7003_C84815.pdf
[gAlcsc]:    https://datasheet.lcsc.com/szlcsc/PMSA003-C_C89095.pdf

## Compatibility

MCU                | Tested Works | Doesn't Work | Not Tested  | Examples | Notes
------------------ | :----------: | :----------: | :---------: | -------- | -----
Atmega328  @  8MHz | X |   |   |                    [SoftwareSerial][]                 |
Atmega328  @ 16MHz |   |   | X |                    [SoftwareSerial][]                 |
ATmega168  @  8MHz |   |   | X |                    [SoftwareSerial][]                 |
Atmega168  @ 16MHz |   |   | X |                                                       |
Atmega32u4 @  8MHz |   |   | X |                                                       |
Atmega32u4 @ 16MHz |   |   | X | [HardwareSerial][]                                    |
Atmega2560 @ 16MHz | X |   |   | [HardwareSerial][]                                    |
STM32f103c8        |   |   | X |                                                       |
STM32f103cb        |   |   | X | [HardwareSerial][]                                    |
ESP8266            | X |   |   | [HardwareSerial][] [SoftwareSerial][]  [OLED 64x48][] |
ESP32              |   |   | X | [HardwareSerial][]                     [OLED 64x48][] | Serial1 not supported


[SoftwareSerial]: examples/SoftwareSerial/README.md
[HardwareSerial]: examples/HardwareSerial/README.md
[OLED 64x48]:     examples/OLED_64x48/README.md
