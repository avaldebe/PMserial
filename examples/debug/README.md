# PMSerial: debug/main.cpp

This is not really an example, but the code used to test and debug changes to the library.
It is a combination of the [HardwareSerial][] and [SoftwareSerial][] examples,
with calls to some special debugging methods.

[HardwareSerial]: ../HardwareSerial/README.md
[SoftwareSerial]: ../SoftwareSerial/README.md

## Boards tested

- AVR: `uno`, `leonardo`, `megaatmega2560`
- SAMD: `mkrwifi1010`
- STM32: `bluepill_f103c8`, `maple_mini_b20`
- ESP8266: `esp01`, `esp01_1m`, `d1_mini`
- ESP32: `mhetesp32minikit`, `m5stack-core-esp32`

Please note that the ESP8266 boards need [EspSoftwareSerial@>=6.7.1][issue6]

[issue6]: https://github.com/avaldebe/PMserial/issues/6

## Debugging methods

Two additional methods are compiled with the `PMS_DEBUG` directive.

```c++
#define PMS_DEBUG
#include <PMserial.h>
```

`print_buffer` partially decodes the contents of the message buffer
and print the result. The following line writes the message buffer to `Serial`
formatted as zero padded hexadecimals.

```c++
pms.print_buffer(Serial, " %04X");
```

`pms.waited_ms` returns the time spent waiting for the sensor to send new measurements. The following line prints the time waited in ms.

```c++
Serial.println(pms.waited_ms());
```
