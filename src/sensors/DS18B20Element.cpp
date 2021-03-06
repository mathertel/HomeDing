/**
 * @file DS18B20Element.cpp
 *
 * @brief Optional Input Element for the HomeDing Library to read DS18B20 sensors and create actions.
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
 * Changelog: see DS18B20Element.h
 */

#include <Arduino.h>
#include <Board.h>
#include <Element.h>

#include <sensors/DS18B20Element.h>


/**
 * @brief static factory function to create a new DS18B20Element
 * @return DS18B20Element* created element
 */
Element *DS18B20Element::create()
{
  return (new DS18B20Element());
} // create()


/**
 * @brief Set a parameter or property to a new value or start an action.
 */
bool DS18B20Element::set(const char *name, const char *value)
{
  bool ret = SensorElement::set(name, value);

  if (!ret) {
    if (_stricmp(name, PROP_PIN) == 0) {
      _pin = _atopin(value);
      ret = true;

    } else if (_stricmp(name, ACTION_ONTEMPERATURE) == 0) {
      _tempAction = value;
      ret = true;
    } // if
  } // if
  return (ret);
} // set()


/**
 * @brief Activate the DS18B20Element.
 */
void DS18B20Element::start()
{
  // TRACE("start()");
  if (_pin >= 0) {
    _oneWire = new (std::nothrow) OneWire(_pin);

    if (_oneWire) {
      // search first device
      _oneWire->reset_search();
      if (_oneWire->search(_addr)) {
        // addr[0] should be 0x28 on a DS18B20 sensor.
        // TRACE("Address: %02x%02x.%02x%02x.%02x%02x.%02x%02x", _addr[0], _addr[1], _addr[2], _addr[3], _addr[4], _addr[5], _addr[6], _addr[7]);
        SensorElement::start();
      }
    } else {
      // TRACE("no address.");
    }
  } // if
} // start()


// ===== OneWire commands and timings
#define COMMAND_CONVERT 0x44 // This command initiates a temperature conversion.
#define COMMAND_READ_SCRATCHPAD 0xbe // This command initiates reading the content of the scratchpad.
#define CONVERT_MSECS 750 // time in msec for 12 bit conversion

/** return true when reading a probe is done.
  * return any existing value or empty for no data could be read. */
bool DS18B20Element::getProbe(String &values)
{
  // LOGGER_EINFO("getProbe()");
  bool newData = false;


  if (_isReady == 0) {
    // start conversion now
    // LOGGER_EINFO("convert");
    _oneWire->reset();
    _oneWire->select(_addr);
    _oneWire->write(COMMAND_CONVERT, 1); // start conversion
    _isReady = millis() + CONVERT_MSECS;

  } else if (_isReady < millis()) {
    // fetch data from sensor
    // LOGGER_EINFO("fetch");
    uint8_t data[12];

    _oneWire->reset();
    _oneWire->select(_addr);
    _oneWire->write(COMMAND_READ_SCRATCHPAD); // Read Scratchpad

    // read all 9 scratchpad bytes
    for (int i = 0; i < 9; i++) {
      data[i] = _oneWire->read();
    }
    // TRACE("Data: %02x %02x %02x %02x", data[0], data[1], data[2], data[3]);

    // no checksum verified
    int temp = (data[1] << 8) | data[0];

    // TRACE("raw temperature: %d", temp);
    values = String((float)temp / 16, 2);

    _isReady = 0;
    newData = true;
  } // if

  return (newData);
} // getProbe()


void DS18B20Element::sendData(String &values)
{
  // dispatch value.
  // TRACE("sending %s", values.c_str());
  _board->dispatch(_tempAction, values);
} // sendData()


void DS18B20Element::pushState(
    std::function<void(const char *pName, const char *eValue)> callback)
{
  SensorElement::pushState(callback);
  callback("temperature", _lastValues.c_str());
} // pushState()


// ===== private functions =====

// End
