# PMSerial

Arduino library for PM sensors with serial interface

## Sensors

[Plantower][]      | Tested Works | Doesn't Work | Not Tested  | Datasheet | Notes
------------------ | :----------: | :----------: | :---------: | --------- | -----
PMS1003 (aka G1)   |   |   | X | [en][g1aqmd],  [cn][g1lcsc] |
PMS3003 (aka G3)   | X |   |   | [en][g3aqmon], [cn][g3lcsc] | No passive mode read
PMS5003 (aka G5)   | X |   |   | [en][g5aqmd],  [cn][g5lcsc] |
PMS7003 (aka G7)   | X |   |   |                [cn][g7lcsc] |
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

## Usage

### PMSx003 on HardwareSerial

```arduino
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

Setup on different MCUs is covered on the [HardwareSerial][] example.

### PMSx003 on SoftwareSerial

```arduino
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

Setup on different MCUs is covered on the [SoftwareSerial][] example.

### PMSx003 message/protocol

With the exemption of the PMS3003, all supported sensors
transmit particulate matter (PM) and number concentrations (NC)
measurements in a 32 byte long message.
The PMS3003 only transmit PM measurements in a 24 byte message.

The `PMSx003` message/protocol on the examples will
determine the message length automatically.
The available protocols are:

message/protocol  | length   | PM   | NC   | sensors (aliases)
----------------- | -------- | :--: | :--: | -----------------
`PLANTOWER_32B`   | 32 bytes | X    | X    | `PMS1003`, `PMS5003`, `PMS7003`, `PMSA003`
`PLANTOWER_24B`   | 24 bytes | X    |      | `PMS3003`
`PLANTOWER_AUTO`  | self discovery| X |auto| `PMSx003`

### PMSx003 reading

The `pms.read()`  method will request a new measurement set from the sensor and decode the sensor message.
The `has_particulate_matter()`/`has_number_concentration()` methods indicate if the message was valid
and PM/NC measurements were successfully decoded.

### PMSx003 measurements

All measurements found in the sensor message
will be decoded and stored in following member variables:

variable | type[len]  | measurement | particle diameter | unit     | Notes
-------- | ---------- | ---------- | ----------------- | ----     | -----
`pm01`   | `uint16_t` | PM         | <= 1.0 µm         | µg/m3    | PM1.0, ultra fine particles
`pm25`   | `uint16_t` | PM         | <= 2.5 µm         | µg/m3    | PM2.5, fine particles
`pm10`   | `uint16_t` | PM         | <= 10  µm         | µg/m3    | PM10
`n0p3`   | `uint16_t` | NC         | >= 0.3 µm         | #/100cm3 |
`n0p5`   | `uint16_t` | NC         | >= 0.5 µm         | #/100cm3 |
`n1p0`   | `uint16_t` | NC         | >= 1.0 µm         | #/100cm3 |
`n2p5`   | `uint16_t` | NC         | >= 2.5 µm         | #/100cm3 |
`n5p0`   | `uint16_t` | NC         | >= 5.0 µm         | #/100cm3 |
`n10p0`  | `uint16_t` | NC         | >= 10  µm         | #/100cm3 |
`pm`     |`uint16_t[3]`|PM         | <= 1,2.5,10 µm    | µg/m3    | array containing `pm01,pm25,pm10`
`nc`     |`uint16_t[6]`|NC         | >= 0.3,0.5,1,5,10 µm|#/100cm3| array containing `n0p3,..,n10p0`
`data`   |`uint16_t[9]`|PM/NC      | | | all PM/NC data `pm01,..,n10p0`

For an efficient use of memory, the `data`, `pm` and `nc` arrays and the
individual measurement variables are implemented with a union/struct combination.
See the [examples](examples/) for an full PM/NC output.

### PMSx003 status/error codes

The `pms.status` member variable contains the status and eventual error code
resulting from the last sensor read.
The available status/error codes and pre-defined error messages are:

status/error code       | error message  | pre-defined text
----------------------- | -------------- | -----------------
`pms.OK`                |
`pms.ERROR_TIMEOUT`     | `PMS_ERROR_TIMEOUT`     | "Sensor read timeout"
`pms.ERROR_PMS_TYPE`    | `PMS_ERROR_PMS_TYPE`    | "Wrong PMSx003 sensor type"
`pms.ERROR_MSG_UNKNOWN` | `PMS_ERROR_MSG_UNKNOWN` | "Unknown message protocol"
`pms.ERROR_MSG_HEADER`  | `PMS_ERROR_MSG_HEADER`  | "Incomplete message header"
`pms.ERROR_MSG_BODY`    | `PMS_ERROR_MSG_BODY`    | "Incomplete message body"
`pms.ERROR_MSG_START`   | `PMS_ERROR_MSG_START`   | "Wrong message start"
`pms.ERROR_MSG_LENGTH`  | `PMS_ERROR_MSG_LENGTH`  | "Message too long"
`pms.ERROR_MSG_CKSUM`   | `PMS_ERROR_MSG_CKSUM`   | "Wrong message checksum"

For easy of use, the error message are pre-defined with `#define`.
See the [examples](examples/) for an error handling implementation.

## Contribute back

If you have read this far, this is the library for your project
or you you can not figure out how to use it. In any case, I can use your help.

If you find any typos or something is not clear, please go to issue [#3][GH3] and leave a message.
Reopen the issue if needed.

If you want something a bit more challenging. I would appreciate more example projects.
See issue [#4][GH4] for inspiration. PRs are welcomed.

[GH3]: https://github.com/avaldebe/PMserial/issues/4
[GH4]: https://github.com/avaldebe/PMserial/issues/4