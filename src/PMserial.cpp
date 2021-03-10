/* SerialPM
 Arduino library for PM sensors with serial interface
  PMS1003 aka G1
  PMS3003 aka G2
  PMS5003 aka G5
  PMS7003 aka G7
  PMSA003 aka G10
*/

#include <PMserial.h>

/* Sensor data format:
  https://github.com/avaldebe/AQmon/blob/master/lua_modules/pms3003.lua

PMS2003, PMS3003:
  24 byte long messages via UART 9600 8N1 (3.3V TTL).
DATA(MSB,LSB): Message header (4 bytes), 2 pairs of bytes (MSB,LSB)
  -1(  1,  2): Begin message       (hex:424D, ASCII 'BM')
   0(  3,  4): Message body length (hex:0014, decimal 20)
DATA(MSB,LSB): Message body (28 bytes), 10 pairs of bytes (MSB,LSB)
   1(  5,  6): PM 1.0 [ug/m3] (TSI standard)
   2(  7,  8): PM 2.5 [ug/m3] (TSI standard)
   3(  9, 10): PM 10. [ug/m3] (TSI standard)
   4( 11, 12): PM 1.0 [ug/m3] (std. atmosphere)
   5( 13, 14): PM 2.5 [ug/m3] (std. atmosphere)
   6( 15, 16): PM 10. [ug/m3] (std. atmosphere)
   7( 17, 18): no idea.
   8( 19, 19): no idea.
   9( 21, 22): no idea.
  10( 23, 24): cksum=byte01+..+byte22
PMS1003, PMS5003, PMS7003:
  32 byte long messages via UART 9600 8N1 (3.3V TTL).
DATA(MSB,LSB): Message header (4 bytes), 2 pairs of bytes (MSB,LSB)
  -1(  1,  2): Begin message       (hex:424D, ASCII 'BM')
   0(  3,  4): Message body length (hex:001C, decimal 28)
DATA(MSB,LSB): Message body (28 bytes), 14 pairs of bytes (MSB,LSB)
   1(  5,  6): PM 1.0 [ug/m3] (TSI standard)
   2(  7,  8): PM 2.5 [ug/m3] (TSI standard)
   3(  9, 10): PM 10. [ug/m3] (TSI standard)
   4( 11, 12): PM 1.0 [ug/m3] (std. atmosphere)
   5( 13, 14): PM 2.5 [ug/m3] (std. atmosphere)
   6( 15, 16): PM 10. [ug/m3] (std. atmosphere)
   7( 17, 18): num. particles with diameter > 0.3 um in 100 cm3 of air
   8( 19, 19): num. particles with diameter > 0.5 um in 100 cm3 of air
   9( 21, 22): num. particles with diameter > 1.0 um in 100 cm3 of air
  10( 23, 24): num. particles with diameter > 2.5 um in 100 cm3 of air
  11( 25, 26): num. particles with diameter > 5.0 um in 100 cm3 of air
  12( 27, 28): num. particles with diameter > 10. um in 100 cm3 of air
  13( 29, 30): Reserved
  14( 31, 32): cksum=byte01+..+byte30
*/

const uint8_t
    TSI_START = 4,  // PM [ug/m3] (TSI standard)
    ATM_START = 10, // PM [ug/m3] (std. atmosphere)
    NUM_START = 16; // num. particles in 100 cm3 of air

const uint8_t
    msgLen = 7,
    //act[msgLen] = {0x42,0x4D,0xE1,0x00,0x01,0x01,0x71}, // set active mode
    slp[msgLen] = {0x42,0x4D,0xE4,0x00,0x00,0x01,0x73}, // sleep
    wak[msgLen] = {0x42,0x4D,0xE4,0x00,0x01,0x01,0x74}, // wake
    cfg[msgLen] = {0x42, 0x4D, 0xE1, 0x00, 0x00, 0x01, 0x70}, // set passive mode
    trg[msgLen] = {0x42, 0x4D, 0xE2, 0x00, 0x00, 0x01, 0x71}; // passive mode read

void SerialPM::init()
{
#ifdef ESP32
  if ((hwSerial == serModeHardware) && rx && tx)
    static_cast<HardwareSerial *>(uart)->begin(9600, SERIAL_8N1, rx, tx);
#else

#ifdef HAS_HW_SERIAL
  if (hwSerial == serModeHardware)
    static_cast<HardwareSerial *>(uart)->begin(9600, SERIAL_8N1);
#endif

#endif // ESP32

#ifdef HAS_SW_SERIAL
  if (hwSerial == serModeSoftware)
    static_cast<SoftwareSerial *>(uart)->begin(9600);
#endif

  uart->write(cfg, msgLen); // set passive mode
  uart->flush();
  delay(max_wait_ms * 2);
  while (uart->available())
  {
    uart->read(); // empty the RX buffer
  }
}

void SerialPM::sleep() {
  uart->write(slp, msgLen);  // sleep mode 
  uart->flush();
  delay(max_wait_ms * 2);
}
void SerialPM::wake() {
  uart->write(wak, msgLen);  // wake mode 
  uart->flush();
  delay(max_wait_ms * 2);
}

SerialPM::STATUS SerialPM::trigRead()
{
  #ifdef HAS_SW_SERIAL
  if (hwSerial == serModeSoftware)
    static_cast<SoftwareSerial *>(uart)->listen(); // when you want to listen on a port, explicitly select it. (https://www.arduino.cc/en/Tutorial/LibraryExamples/TwoPortReceive)
  #endif

  while (uart->available())
  {
    uart->read(); // empty the RX buffer
  }
  uart->write(trg, msgLen); // passive mode read
  uart->flush();

  // wait for the mesage header
  const size_t headLen = 4;     // message header length
  uint32_t start_ms = millis(); // start waiting time
  do
  {                                // ~650ms to complete a measurements
    delay(10);                     // wait up to max_wait_ms
    wait_ms = millis() - start_ms; // time waited so far
  } while (size_t(uart->available()) < headLen && wait_ms < max_wait_ms);

  // we should an answer/message after 650ms
  if (!uart->available())
    return ERROR_TIMEOUT;

  // read message header
  nbytes = uart->readBytes(&buffer[0], headLen);
  if (nbytes != headLen)
    return ERROR_MSG_HEADER;

  // message header starts with 'BM'
  if (buff2word(0) != 0x424D)
    return ERROR_MSG_START;

  // check message length against stated sensor type
  size_t bodyLen = buff2word(2);         // message body length
  size_t messageLen = headLen + bodyLen; // full message length
  PMS sensor;
  switch (messageLen)
  {
  case 24:
    sensor = PLANTOWER_24B;
    break;
  case 32:
    sensor = PLANTOWER_32B;
    break;
  case 40:
    sensor = PLANTOWER_40B;
    break;
  default:
    return ERROR_MSG_UNKNOWN;
  }
  // self discovery
  if (pms == PLANTOWER_AUTO)
    pms = sensor;
  // check sensor type
  if (pms != sensor)
    return ERROR_PMS_TYPE;

  // full message should fit in the buffer
  if (messageLen > BUFFER_LEN)
    return ERROR_MSG_LENGTH;

  // wait for the message body
  do
  {                                // ~650ms to complete a measurements
    delay(10);                     // wait up to max_wait_ms
    wait_ms = millis() - start_ms; // time waited so far
  } while (size_t(uart->available()) < bodyLen && wait_ms < max_wait_ms);

  // we should an answer/message after 650ms
  if (!uart->available())
    return ERROR_TIMEOUT;

  // read message body
  nbytes += uart->readBytes(&buffer[headLen], bodyLen);
  if (nbytes != messageLen)
    return ERROR_MSG_BODY;

  if (!checkBuffer(messageLen))
    return ERROR_MSG_CKSUM;

  return OK;
}

bool SerialPM::checkBuffer(size_t bufferLen)
{
  uint16_t cksum = buff2word(bufferLen - 2);
  for (uint8_t n = 0; n < bufferLen - 2; n++)
  {
    cksum -= buffer[n];
  }

  return (cksum == 0);
}

void SerialPM::decodeBuffer(bool tsi_mode, bool truncated_num)
{
  uint8_t bin, n;
  if (!has_particulate_matter())
    return;
  for (bin = 0, n = tsi_mode ? TSI_START : ATM_START; bin < 3; bin++, n += 2)
  {
    pm[bin] = buff2word(n);
  }

  if (!has_number_concentration())
    return;
  for (bin = 0, n = NUM_START; bin < 6; bin++, n += 2)
  {
    nc[bin] = buff2word(n); // number particles w/diameter > r_bin
  }

  switch (pms)
  {
  case PMS5003S:
    hcho = buff2word(28) * 1e-3;
    break;
  case PMS5003T:
    temp = int16_t(n5p0) * 1e-1 + temp_offset; // cast to signed integer 16bits
    rhum = n10p0 * 1e-1 + rhum_offset;
    n5p0 = 0;
    n10p0 = 0;
    break;
  case PMS5003ST:
    hcho = buff2word(28) * 1e-3;
    temp = int16_t(buff2word(30)) * 1e-1 + temp_offset; // cast to signed integer 16bits
    rhum = buff2word(32) * 1e-1 + rhum_offset;
    break;
  default:
    break;
  }

  if (!truncated_num)
    return;
  for (bin = 0; bin < 5; bin++)
  {
    nc[bin] -= nc[bin + 1]; // de-accumulate number concentrations
  }
}

SerialPM::STATUS SerialPM::read(bool tsi_mode, bool truncated_num)
{
  status = trigRead();                   // read comand on passive mode
  decodeBuffer(tsi_mode, truncated_num); // decode message only if buffer checks out
  return status;
}
