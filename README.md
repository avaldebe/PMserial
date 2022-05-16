# PMSerial

Arduino library for PM sensors with serial interface

[![PlatformIO CI](https://github.com/avaldebe/PMserial/actions/workflows/CI.yml/badge.svg)](https://github.com/avaldebe/PMserial/actions/workflows/CI.yml)
[![GitHub issues](https://img.shields.io/github/issues/avaldebe/PMserial)](https://github.com/avaldebe/PMSerial/issues)
[![GitHub license](https://img.shields.io/github/license/avaldebe/PMserial)](https://github.com/avaldebe/PMserial/blob/master/LICENSE)


## Sensors

| [Plantower][]        | Tested Works | Doesn't Work | Not Tested | Datasheet                   | Notes                |
| -------------------- | :----------: | :----------: | :--------: | --------------------------- | -------------------- |
| PMS1003 (aka G1)     |              |              |     X      | [en][g1aqmd],  [cn][g1lcsc] |
| PMS3003 (aka G3)     |      X       |              |            | [en][g3aqmon], [cn][g3lcsc] | No passive mode read |
| PMS5003 (aka G5)     |      X       |              |            | [en][g5aqmd],  [cn][g5lcsc] |
| PMS5003S (aka G5S)   |              |              |     X      | [cn][g5slcsc]               |
| PMS5003T (aka G5T)   |              |              |     X      |
| PMS5003ST (aka G5ST) |              |              |     X      |
| PMS7003 (aka G7)     |      X       |              |            | [cn][g7lcsc]                |
| PMSA003 (aka G10)    |      X       |              |            | [cn][gAlcsc]                |

[plantower]: http://www.plantower.com/
[g1aqmd]:    http://www.aqmd.gov/docs/default-source/aq-spec/resources-page/plantower-pms1003-manual_v2-5.pdf?sfvrsn=2
[g5aqmd]:    http://www.aqmd.gov/docs/default-source/aq-spec/resources-page/plantower-pms5003-manual_v2-3.pdf?sfvrsn=2
[g3aqmon]:   https://github.com/avaldebe/AQmon/raw/master/Documents/PMS3003_LOGOELE.pdf
[g5aqmon]:   https://github.com/avaldebe/AQmon/raw/master/Documents/PMS5003_LOGOELE.pdf
[g1lcsc]:    https://datasheet.lcsc.com/szlcsc/PMS1003_C89289.pdf
[g3lcsc]:    https://datasheet.lcsc.com/szlcsc/PMS3003_C87024.pdf
[g5lcsc]:    https://datasheet.lcsc.com/szlcsc/PMS5003_C91431.pdf
[g5slcsc]:   https://datasheet.lcsc.com/szlcsc/PMS5003S_C91432.pdf
[g7lcsc]:    https://datasheet.lcsc.com/szlcsc/PMS7003_C84815.pdf
[gAlcsc]:    https://datasheet.lcsc.com/szlcsc/PMSA003-A_C132744.pdf

## Compatibility

| MCU                | Tested Works | Doesn't Work | Not Tested | Examples                                              | Notes                                  |
| ------------------ | :----------: | :----------: | :--------: | ----------------------------------------------------- | -------------------------------------- |
| ATmega168  @  8MHz |              |              |     X      | [SoftwareSerial][]                                    |
| Atmega168  @ 16MHz |              |              |     X      |                                                       | 5V boards need 3.3V/5V level shifter   |
| Atmega328  @  8MHz |      X       |              |            | [SoftwareSerial][]                                    |
| Atmega328  @ 16MHz |              |              |     X      |                                                       | 5V boards need 3.3V/5V level shifter   |
| Atmega32u4 @  8MHz |              |              |     X      |                                                       |
| Atmega32u4 @ 16MHz |              |              |     X      | [HardwareSerial][]                                    | 5V boards need 3.3V/5V level shifter   |
| Atmega2560 @ 16MHz |      X       |              |            | [HardwareSerial][]                                    | 5V boards need 3.3V/5V level shifter   |
| STM32f103c8        |      X       |              |            | [HardwareSerial][]                                    |
| STM32f103cb        |              |              |     X      | [HardwareSerial][]                                    |
| ESP8266            |      X       |              |            | [HardwareSerial][] [SoftwareSerial][]  [OLED 64x48][] | [needs EspSoftwareSerial@>=6.7.1][GH6] |
| ESP32              |      X       |              |            | [HardwareSerial][] [SoftwareSerial][]  [OLED 64x48][] | [Serial1 as SoftwareSerial][GH7]       |

[SoftwareSerial]: examples/SoftwareSerial/README.md
[HardwareSerial]: examples/HardwareSerial/README.md
[OLED 64x48]:     examples/OLED_64x48/README.md

## Usage

### `PMSx003` sensor type

The `PMSx003` sensor type will infer the sensor type from the message header.
The sensor type inference does not cover the `PMS5003S` and `PMS5003T` variants, see [#10][GH10].
`PMS5003S` and `PMS5003T` sensors need to be declared explicitly on the `SerialPM` constructor.

### PMSx003 on HardwareSerial

```c++
#include <PMserial.h>
SerialPM pms(PMSx003, Serial);  // PMSx003, UART

void setup() {
  Serial.begin(9600);
  pms.init();                   // config serial port
}

void loop() {
  pms.read();                   // read the PM sensor
  Serial.print(F("PM1.0 "));Serial.print(pms.pm01);Serial.print(F(", "));
  Serial.print(F("PM2.5 "));Serial.print(pms.pm25);Serial.print(F(", "));
  Serial.print(F("PM10 ")) ;Serial.print(pms.pm10);Serial.println(F(" [ug/m3]"));
  delay(10000);                 // wait for 10 seconds
}
```

Setup for different MCU is covered on the [HardwareSerial][] example.

### PMSx003 on SoftwareSerial

```c++
#include <PMserial.h>
SerialPM pms(PMSx003, 10, 11);  // PMSx003, RX, TX

void setup() {
  Serial.begin(9600);
  pms.init();                   // config serial port
}

void loop() {
  pms.read();                   // read the PM sensor
  Serial.print(F("PM1.0 "));Serial.print(pms.pm01);Serial.print(F(", "));
  Serial.print(F("PM2.5 "));Serial.print(pms.pm25);Serial.print(F(", "));
  Serial.print(F("PM10 ")) ;Serial.print(pms.pm10);Serial.println(F(" [ug/m3]"));
  delay(10000);                 // wait for 10 seconds
}
```

Setup for different MCU is covered on the [SoftwareSerial][] example.

### ESP32 Serial1

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
SerialPM pms(PMSx003, <RX>, <TX>);
```

The [SoftwareSerial example][esp32hw] uses Serial1 on pins 23 (RX) and 19 (TX).
The [HardwareSerial example][esp32hw] uses Serial2.

[esp32hw]: examples/HardwareSerial/README.md#esp32-minikit
[esp32sw]: examples/SoftwareSerial/README.md#esp32-minikit

## Advanced usage

### Sensor message/protocol

With the exemption of the `PMS3003` and `PMS5003ST`, all supported sensors
transmit particulate matter (PM) and number concentrations (NC)
measurements in a 32 byte long message.
The `PMS3003` only transmit PM measurements in a 24 byte message.
The `PMS5003ST` transmit PM, NC, temperature (T), relative humidity (RH) and formaldehyde concentration (HCHO) measurements
on a 40 byte message.

On the examples, the `PMSx003` sensor type  will infer the sensor type from the message header
and select the message length and decoding protocol accordingly.
The supported protocols are:

| message/protocol  | length         |  PM   |  NC   | T & RH | HCHO  | sensors (aliases)                          |
| ----------------- | -------------- | :---: | :---: | :----: | :---: | ------------------------------------------ |
| `PLANTOWER_AUTO`  | self discovery |   3   | auto  |        |       | `PMSx003`                                  |
| `PLANTOWER_24B`   | 24 bytes       |   3   |       |        |       | `PMS3003`                                  |
| `PLANTOWER_32B`   | 32 bytes       |   3   |   6   |        |       | `PMS1003`, `PMS5003`, `PMS7003`, `PMSA003` |
| `PLANTOWER_32B_S` | 32 bytes       |   3   |   6   |        |   X   | `PMS5003S`, can not be self discovered     |
| `PLANTOWER_32B_T` | 32 bytes       |   3   |   4   |   X    |       | `PMS5003T`, can not be self discovered     |
| `PLANTOWER_40B`   | 40 bytes       |   3   |   6   |   X    |   X   | `PMS5003ST`                                |

### Additional measurements

The `pms.read()`  method will request a new measurement set from the sensor and decode the sensor message.
The `has_particulate_matter()`/`has_number_concentration()` methods indicate if the message was valid
and PM/NC measurements were successfully decoded.
Similarly the `has_temperature_humidity`/`has_formaldehyde` methods indicate if the message was valid
and T & RH/HCHO measurements were successfully decoded, however
this kind of additional measurements are only available on `PMS5003S`, `PMS5003T` and `PMS5003ST` sensors.

### Decoded measurements

All measurements found in the sensor message
will be decoded and stored in following member variables:

| variable | type[len]     | measurement | particle diameter    | unit      | Notes                             |
| -------- | ------------- | ----------- | -------------------- | --------- | --------------------------------- |
| `pm01`   | `uint16_t`    | PM          | <= 1.0 µm            | µg/m³     | PM1.0, ultra fine particles       |
| `pm25`   | `uint16_t`    | PM          | <= 2.5 µm            | µg/m³     | PM2.5, fine particles             |
| `pm10`   | `uint16_t`    | PM          | <= 10  µm            | µg/m³     | PM10                              |
| `n0p3`   | `uint16_t`    | NC          | >= 0.3 µm            | #/100 cm³ |
| `n0p5`   | `uint16_t`    | NC          | >= 0.5 µm            | #/100 cm³ |
| `n1p0`   | `uint16_t`    | NC          | >= 1.0 µm            | #/100 cm³ |
| `n2p5`   | `uint16_t`    | NC          | >= 2.5 µm            | #/100 cm³ |
| `n5p0`   | `uint16_t`    | NC          | >= 5.0 µm            | #/100 cm³ |
| `n10p0`  | `uint16_t`    | NC          | >= 10  µm            | #/100 cm³ |
| `pm`     | `uint16_t[3]` | PM          | <= 1,2.5,10 µm       | µg/m³     | array containing `pm01,pm25,pm10` |
| `nc`     | `uint16_t[6]` | NC          | >= 0.3,0.5,1,5,10 µm | #/100cm³  | array containing `n0p3,..,n10p0`  |
| `data`   | `uint16_t[9]` | PM/NC       |                      |           | all PM/NC data `pm01,..,n10p0`    |
| `temp`   | `float`       | T           |                      | °C        | temperature                       |
| `rhum`   | `float`       | RH          |                      | %         | relative humidity                 |
| `hcho`   | `float`       | HCHO        |                      | mg/m³     | formaldehyde concentration        |
| `extra`  | `float[3]`    | T/RH/HCHO   |                      |           | array containing `temp,rhum,hcho` |

For an efficient use of memory, the `data`, `pm` and `nc` arrays and the
individual measurement variables are implemented with a union/struct combination.
See the [examples](examples/) for an full PM/NC output.

### Status and error codes

The `pms.status` member variable contains the status and eventual error code
resulting from the last sensor read.
The available status/error codes and pre-defined error messages are:

| status/error code       | error message           | pre-defined text            |
| ----------------------- | ----------------------- | --------------------------- |
| `pms.OK`                |
| `pms.ERROR_TIMEOUT`     | `PMS_ERROR_TIMEOUT`     | "Sensor read timeout"       |
| `pms.ERROR_PMS_TYPE`    | `PMS_ERROR_PMS_TYPE`    | "Wrong PMSx003 sensor type" |
| `pms.ERROR_MSG_UNKNOWN` | `PMS_ERROR_MSG_UNKNOWN` | "Unknown message protocol"  |
| `pms.ERROR_MSG_HEADER`  | `PMS_ERROR_MSG_HEADER`  | "Incomplete message header" |
| `pms.ERROR_MSG_BODY`    | `PMS_ERROR_MSG_BODY`    | "Incomplete message body"   |
| `pms.ERROR_MSG_START`   | `PMS_ERROR_MSG_START`   | "Wrong message start"       |
| `pms.ERROR_MSG_LENGTH`  | `PMS_ERROR_MSG_LENGTH`  | "Message too long"          |
| `pms.ERROR_MSG_CKSUM`   | `PMS_ERROR_MSG_CKSUM`   | "Wrong message checksum"    |

For easy of use, the error message are pre-defined with `#define`.
See the [examples](examples/) for error handling implementation.

## Temperature and humidity offset correction (Optional)

Some sensors have a small deviation with temperature and humidity readings.
The compensate is possible with a correction offset defined by set_temp_offset() and set_rhum_offset().
The result will be reading + offset. Default offset values are O.

Example:
```c++
set_temp_offset(-0.6);
set_rhum_offset(2);
```

## SoftwareSerial Compatibility

Some libraries (e.g [Arduino-SDI-12](https://github.com/EnviroDIY/Arduino-SDI-12)) conflict with SoftwareSerial library, which is loaded by default for
some boards. In order to disable SoftwareSerial compatibility, define `NO_SW_SERIAL_REQUIRED` before
including the library. For example:

```c++
#define NO_SW_SERIAL_REQUIRED
#include <PMSerial.h>
```

## Contribute

If you have read this far, this is the library for your project
or you can not figure out how to use it. In any case, I can use your help.

If you find any typos or something is not clear, please go to issue [#3][GH3] and leave a message.
Reopen the issue if needed.

If you want something a bit more challenging. I would appreciate more example projects.
See issue [#4][GH4] for inspiration. PRs are welcomed.

[GH3]: https://github.com/avaldebe/PMserial/issues/3
[GH4]: https://github.com/avaldebe/PMserial/issues/4

## Changelog

- 1.2.0
  - Avoid loading `SoftwareSerial` library with `-D NO_SW_SERIAL_REQUIRED` compilation flag  [PR24][]
  - Update OLED library used on examples
  - Enable STM32F103C8 Serial1
- 1.1.1
  - Fix SoftwareSerial dangling pointer [#19][GH19]/[PR20][]
  - ESP8266 no longer requires EspSoftwareSerial
- 1.1.0
  - Support `atmelsam` devices [#8][GH8]/[PR12][]
  - Support the PMS5003 S/ST/T sensors [#10][GH10],[#16][GH16]/[PR17][]
  - Support `sleep`/`wake` commands [#14][GH14]/[PR15][]
  - Use Serial1 as "SoftwareSerial" for ESP32, [#7][GH7]/[PR9][]
- 1.0.1
  - Fix broken SoftwareSerial for ESP8266, [#6][GH6]
  - ESP8266 use EspSoftwareSerial@>=6.7.1
- 1.0.0
  - first complete release

[GH6]:  https://github.com/avaldebe/PMserial/issues/6
[GH7]:  https://github.com/avaldebe/PMserial/issues/7
[GH8]:  https://github.com/avaldebe/PMserial/issues/8
[PR9]:  https://github.com/avaldebe/PMserial/pull/9
[GH10]: https://github.com/avaldebe/PMserial/issues/10
[PR12]: https://github.com/avaldebe/PMserial/pull/12
[GH14]: https://github.com/avaldebe/PMserial/issues/14
[PR15]: https://github.com/avaldebe/PMserial/pull/15
[GH16]: https://github.com/avaldebe/PMserial/issues/16
[PR17]: https://github.com/avaldebe/PMserial/pull/17
[GH19]: https://github.com/avaldebe/PMserial/issues/19
[PR20]: https://github.com/avaldebe/PMserial/pull/20
[PR24]: https://github.com/avaldebe/PMserial/pull/24
