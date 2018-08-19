/**
 * @file ValueElement.cpp
 * @brief Element Template class.
 *
 * @author Matthias Hertel, https://www.mathertel.de
 *
 * @Copyright Copyright (c) by Matthias Hertel, https://www.mathertel.de.
 *
 * This work is licensed under a BSD style license.
 * https://www.mathertel.de/License.aspx.
 *
 * More information on https://www.mathertel.de/Arduino
 *
 * Changelog:see ValueElement.h
 */


#include "ValueElement.h"
#include "ElementRegistry.h"

#define LOGGER_MODULE "value"
#include "Logger.h"

/**
 * @brief static factory function to create a new ValueElement
 * @return ValueElement* created element
 */
Element *ValueElement::create()
{
  LOGGER_TRACE("create()");
  return (new ValueElement());
} // create()


/**
 * @brief Set a parameter or property to a new value or start an action.
 */
bool ValueElement::set(const char *name, const char *value)
{
  LOGGER_TRACE("set(%s, %s)", name, value);
  bool ret = true;

  if (_stricmp(name, "value") == 0) {
    _setValue(atoi(value));

  } else if (_stricmp(name, "min") == 0) {
    _min = atoi(value);

  } else if (_stricmp(name, "max") == 0) {
    _max = atoi(value);

  } else if (_stricmp(name, "onchange") == 0) {
    _changeAction = value;

  } else if (_stricmp(name, "onchange") == 0) {
    _changeAction = value;

  } else if (_stricmp(name, "up") == 0) {
    _setValue(_value += atoi(value));

  } else if (_stricmp(name, "down") == 0) {
    _setValue(_value -= atoi(value));

  } else {
    ret = Element::set(name, value);
  } // if

  return (ret);
} // set()


void ValueElement::start()
{
  Element::start();
  // send out the actual defined value
  _setValue(_value);
} // start()


/**
 * @brief push the current value of all properties to the callback.
 */
void ValueElement::pushState(
    std::function<void(const char *pName, const char *eValue)> callback)
{
  Element::pushState(callback);
  callback("value", String(_value).c_str());
} // pushState()


// set a new value, maybe adjust to range
void ValueElement::_setValue(int newValue)
{
  _value = newValue;

  if (_value < _min)
    _value = _min;

  if (_value > _max)
    _value = _max;

  if (active) {
    char tmp[10];
    itoa(_value, tmp, sizeof(tmp));

    _board->dispatch(_changeAction, tmp);
  } // if
} // _setValue()

// End