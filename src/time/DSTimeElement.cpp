/**
 * @file DSTimeElement.cpp
 *
 * @brief Core Element for the HomeDing Library to get the actual local time
 * using a RTC DS3231 chip.
 *
 * @author Matthias Hertel, https://www.mathertel.de
 *
 * @Copyright Copyright (c) by Matthias Hertel, https://www.mathertel.de.
 *
 * This work is licensed under a BSD style license,
 * https://www.mathertel.de/License.aspx.
 *
 * More information on https://www.mathertel.de/Arduino.
 *
 * https://abzman2k.wordpress.com/2015/09/24/esp8266-rtc-and-ntp/
 *
 * Changelog: see time/DSTimeElement.h
 */

#include <Arduino.h>
#include <Element.h>
#include <Board.h>

#include <ElementRegistry.h>
#include <time/DSTimeElement.h>

#include <Wire.h>
#include <WireUtils.h>

#include <sntp.h>
#include <sys/time.h>
#include <time.h>

extern "C" {
#include "user_interface.h"
}

#define DS3231_ADDRESS 0x68
#define DS3231_REGSTATUS (0x0F)
#define DS3231_REGSTATUS_OSF (0x80)

// Convert byte value to BCD value.
uint8_t DSTimeElement::dec2Bcd(uint8_t val)
{
  return ((val / 10 * 16) + (val % 10));
} // dec2Bcd()


// Convert BCD value to byte value.
uint8_t DSTimeElement::bcd2Dec(uint8_t val)
{
  return ((val / 16 * 10) + (val % 16));
} // bcd2Dec()


void DSTimeElement::_setSystemTime(struct tm *t)
{
  time_t secs = mktime(t);
  struct timeval tv = {.tv_sec = secs, .tv_usec = 0};
  sntp_set_timezone(0);
  settimeofday(&tv, NULL);
} // _setSystemTime()


// get time from the RTC chip
void DSTimeElement::_getDSTime(int adr, struct tm *t)
{
  memset(t, 0, sizeof(struct tm));
  uint8_t data[8];

  WireUtils::read(adr, 0, 7, data);

  t->tm_sec = bcd2Dec(data[0]);
  t->tm_min = bcd2Dec(data[1]);
  t->tm_hour = bcd2Dec(data[2]);
  // data[3]; // day not used.
  t->tm_mday = bcd2Dec(data[4]);
  t->tm_mon = bcd2Dec(data[5]);
  t->tm_year = bcd2Dec(data[6]) + 2000 - 1900;
} // _getDSTime()


// update time in the RTC chip
void DSTimeElement::_setDSTime(int adr, struct tm *t)
{
  uint8_t data[8];

  // save time to registers 0-7
  data[0] = dec2Bcd(t->tm_sec);
  data[1] = dec2Bcd(t->tm_min);
  data[2] = dec2Bcd(t->tm_hour); // always using 24h mode.
  data[3] = 0; // day of week not used.
  data[4] = dec2Bcd(t->tm_mday);
  data[5] = dec2Bcd(t->tm_mon);
  data[6] = dec2Bcd(t->tm_year % 100); // year is alwaysin the range of 20xx

  WireUtils::write(adr, 0, 7, data);
} // _setDSTime()

// http://lcddevice/$board/dstime/0?time=2019-01-19%2018:35:00

// set value using format "2019-02-19 15:13:31"
void DSTimeElement::_setTime(const char *value)
{
  struct tm t;
  char *pEnd = (char *)value;

  LOGGER_INFO("setTime(%s)", value);

  if (strlen(value) == 19) {
    memset(&t, 0, sizeof(struct tm));
    t.tm_year = strtol(pEnd, &pEnd, 10) - 1900;
    if (*pEnd == '-')
      pEnd++;
    t.tm_mon = strtol(pEnd, &pEnd, 10);
    if (*pEnd == '-')
      pEnd++;
    t.tm_mday = strtol(pEnd, &pEnd, 10);
    if ((*pEnd == ' ') || (*pEnd == 'T'))
      pEnd++;
    t.tm_hour = strtol(pEnd, &pEnd, 10);
    if (*pEnd == ':')
      pEnd++;
    t.tm_min = strtol(pEnd, &pEnd, 10);
    if (*pEnd == ':')
      pEnd++;
    t.tm_sec = strtol(pEnd, &pEnd, 10);

    LOGGER_INFO("Got time: %s", asctime(&t));
    _setSystemTime(&t);

    // update chip and reset OSF flag.
    _setDSTime(_address, &t);
    int status = WireUtils::read(_address, DS3231_REGSTATUS);
    WireUtils::write(_address, DS3231_REGSTATUS, status & (~DS3231_REGSTATUS_OSF));
  }
} // _setTime()


/**
 * @brief static factory function to create a new DSTimeElement
 * @return DSTimeElement* created element
 */
Element *DSTimeElement::create()
{
  return (new DSTimeElement());
} // create()


DSTimeElement::DSTimeElement()
{
  // set some defaults
  _address = DS3231_ADDRESS;
  _readTime = (8 * 60 * 60); // every 8 hours
  _nextRead = 0;
} // DSTimeElement()


/**
 * @brief Set a parameter or property to a new value or start an action.
 */
bool DSTimeElement::set(const char *name, const char *value)
{
  bool ret = true;

  if (_stricmp(name, "readtime") == 0) {
    _readTime = _atotime(value);

  } else if (_stricmp(name, "address") == 0) {
    _address = strtol(value, nullptr, 0);

  } else if (_stricmp(name, "time") == 0) {
    // adjust date & time
    _setTime(value);

  } else {
    ret = Element::set(name, value);
  } // if
  return (ret);
} // set()


/**
 * @brief Activate the DSTimeElement.
 */
void DSTimeElement::start()
{
  // LOGGER_ETRACE("start()");
  Wire.begin(_board->I2cSda, _board->I2cScl);

  unsigned long now = _board->getSeconds();

  _nextRead = now + 1; // now + min. 2 sec., don't hurry
  _state = 0;
  Element::start();
} // start()

static void sntp_set_system_time(uint32_t t);

/**
 * @brief check the state of the local time DHT and eventually request a new
 * ntp time.
 */
void DSTimeElement::loop()
{
  unsigned long now = _board->getSeconds();
  struct tm t;

  if ((_state != 1) && (_nextRead < now)) {
    int status = WireUtils::read(DS3231_ADDRESS, DS3231_REGSTATUS);

    // The OSF flag shows if the time is valid / no power lost since last
    // adjustment.
    if (status & DS3231_REGSTATUS_OSF) {
      // no current time
      _state = 0;

    } else {
      _getDSTime(_address, &t);
      _setSystemTime(&t);
      _state = 2;
      _nextRead = now + _readTime;
    }
  } // if
} // loop()


void DSTimeElement::pushState(
    std::function<void(const char *pName, const char *eValue)> callback)
{
  time_t tStamp = sntp_get_current_timestamp();
  char tmp[32];

  Element::pushState(callback);
  strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&tStamp));
  callback("now", tmp);
  callback("wire-address", String(_address).c_str());
} // pushState()

// End.
